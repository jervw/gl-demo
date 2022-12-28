#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mesh.h"
#include "shader.h"

#include <iostream>
#include <map>
#include <string>
#include <vector>

class Model {
  public:
    Model(std::string path, bool gamma = false);

    void draw(Shader shader);

    // model data
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;
    bool gamma_correction;

  private:
    void process_node(aiNode* node, const aiScene* scene);
    Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> load_material_textures(aiMaterial* mat, aiTextureType type,
                                                std::string type_name);

    // utility to load a texture from file
    unsigned int texture_from_file(const char* path, const std::string& directory);
};

#endif
