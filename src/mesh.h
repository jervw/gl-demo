#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"

#include <string>
#include <vector>

class Shader;

class Mesh {
  public:
    Mesh(std::string file, Shader shader);
    ~Mesh();
    void draw();
    void translate(glm::vec3 position);

  private:
    std::string file_;
    GLuint shader_;

    std::vector<glm::vec3> vertices_;
    std::vector<glm::vec2> uvs_;
    std::vector<glm::vec3> normals_;
    GLuint vbo_;
    GLuint vcb_;
    GLuint vao_;
    GLuint model_matrix_id_;
    glm::mat4 translate_matrix_;
    glm::mat4 rotation_matrix_;
    glm::mat4 model_matrix_;

    void load();
};

#endif
