#include "mesh.h"
#include "obj_loader.h"

#include <iostream>

Mesh::Mesh(std::string file, Shader shader) {
    file_ = file;
    model_matrix_ = glm::mat4(1);
    translate_matrix_ = glm::mat4(1);
    rotation_matrix_ = glm::mat4(1);
    shader_ = shader.get_program_id();

    load();
}

Mesh::~Mesh() {
    vertices_.clear();
    uvs_.clear();
    normals_.clear();

    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &vcb_);
    glDeleteVertexArrays(1, &vao_);
}

void Mesh::draw() {
    model_matrix_ = translate_matrix_ * rotation_matrix_;
    glUniformMatrix4fv(model_matrix_id_, 1, GL_FALSE, &model_matrix_[0][0]);

    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, vertices_.size());
}

void Mesh::translate(glm::vec3 position) {
    translate_matrix_ = glm::mat4(1);
    translate_matrix_ = glm::translate(translate_matrix_, position);
}

void Mesh::load() {
    if (!load_obj(file_.c_str(), vertices_, uvs_, normals_)) {
        std::cout << "Failed to load mesh from file: " << file_ << std::endl;
        return;
    };

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(glm::vec3), &vertices_[0],
                 GL_STATIC_DRAW);

    glGenBuffers(1, &vcb_);
    glBindBuffer(GL_ARRAY_BUFFER, vcb_);
    glBufferData(GL_ARRAY_BUFFER, normals_.size() * sizeof(glm::vec3), &normals_[0],
                 GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vcb_);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    model_matrix_id_ = glGetUniformLocation(shader_, "mm");
}
