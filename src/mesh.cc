#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
           std::vector<Texture> textures, Material material) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->material = material;

    setup_mesh();
}

// render the mesh
void Mesh::draw(Shader& shader) {
    unsigned int diffuse_nr = 1;
    unsigned int specular_nr = 1;
    unsigned int normal_nr = 1;
    unsigned int height_nr = 1;

    // bind appropriate textures
    for (unsigned int i = 0; i < textures.size(); i++) {
        std::string number;
        std::string name = textures[i].type;

        glActiveTexture(GL_TEXTURE0 + i);

        if (name == "texture_diffuse")
            number = std::to_string(diffuse_nr++);
        else if (name == "texture_specular")
            number = std::to_string(specular_nr++);
        else if (name == "texture_normal")
            number = std::to_string(normal_nr++);
        else if (name == "texture_height")
            number = std::to_string(height_nr++);

        // set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);

        // bind the texture
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // set material properties
    shader.set_vec3("material.ambient", material.ambient);
    shader.set_vec3("material.diffuse", material.diffuse);
    shader.set_vec3("material.specular", material.specular);
    shader.set_float("material.shininess", material.shininess);

    // draw mesh
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // set everything back to defaults
    glActiveTexture(GL_TEXTURE0);
}

// initializes all the buffer objects/arrays
void Mesh::setup_mesh() {
    // clang-format off

    // create buffers/arrays
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);

    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // load data into element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

    glBindVertexArray(0);

    // clang-format on
}
