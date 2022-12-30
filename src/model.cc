#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb_image.h"

#include "model.h"

Model::Model(std::string path) {
    // stb image flip on y axis
    stbi_set_flip_vertically_on_load(true);

    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene =
        importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                                    aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    process_node(scene->mRootNode, scene);

    std::cout << "Model loaded: " << path << std::endl;
}

void Model::draw(Shader shader) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].draw(shader);
    }
}

Material Model::load_material(aiMaterial* mat) {
    Material material;

    aiColor3D color(0.f, 0.f, 0.f);

    // ambient
    mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
    material.ambient = glm::vec3(color.r, color.g, color.b);

    // diffuse
    mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    material.diffuse = glm::vec3(color.r, color.g, color.b);

    // specular
    mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
    material.specular = glm::vec3(color.r, color.g, color.b);

    // shininess
    float shininess;
    mat->Get(AI_MATKEY_SHININESS, shininess);
    material.shininess = shininess;

    return material;
}

void Model::process_node(aiNode* node, const aiScene* scene) {
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(process_mesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        process_node(node->mChildren[i], scene);
    }
}

Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector;

        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        // normals
        if (mesh->mNormals) // does the mesh contain normal vectors?
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }

        // texture coordinates
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.tex_coords = vec;
        } else
            vertex.tex_coords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // now wak through each
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    Material mat = load_material(material);

    // diffuse maps
    std::vector<Texture> diffuse_maps =
        load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

    // specular maps
    std::vector<Texture> specular_maps =
        load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

    // normal maps
    std::vector<Texture> normal_maps =
        load_material_textures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());

    // height maps
    std::vector<Texture> height_maps =
        load_material_textures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), height_maps.begin(), height_maps.end());

    return Mesh(vertices, indices, textures, mat);
}

std::vector<Texture> Model::load_material_textures(aiMaterial* mat, aiTextureType type,
                                                   std::string type_name) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++) {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip) {
            Texture texture;
            texture.id = texture_from_file(str.C_Str(), directory);
            texture.type = type_name;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

unsigned int Model::texture_from_file(const char* path, const std::string& directory) {
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int tex_id;
    glGenTextures(1, &tex_id);

    int width, height, component_count;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &component_count, 0);
    if (data) {
        GLenum format;
        if (component_count == 1)
            format = GL_RED;
        else if (component_count == 3)
            format = GL_RGB;
        else if (component_count == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, tex_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return tex_id;
};
