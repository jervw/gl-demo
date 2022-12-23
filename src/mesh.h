#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

class Mesh {
  public:
    std::string file;
    Mesh(std::string file, GLuint shader);
    ~Mesh();
    void clear();
    void draw_mesh();
    void translate(glm::vec3 position);

  private:
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
    GLuint shader_;

    void load();
};

#endif
