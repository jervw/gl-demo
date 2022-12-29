#ifndef MESH_H
#define MESH_H

#include <GL/glew.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "material.h"
#include "shader.h"

#include <iostream>
#include <string>
#include <vector>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
  public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
         std::vector<Texture> textures, Material material);

    // mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Material material;

    // render the mesh
    void draw(Shader& shader);

  private:
    // render data
    unsigned int vao_, vbo_, ebo_;

    // initializes all the buffer objects/arrays
    void setup_mesh();
};
#endif
