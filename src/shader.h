#ifndef SHADER_H
#define SHADER_H

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>

class Shader {
  public:
    unsigned int ID;
    Shader(const char* vertex_path, const char* fragment_path);
    void use();
    void set_bool(const std::string& name, bool value) const;
    void set_int(const std::string& name, int value) const;
    void set_float(const std::string& name, float value) const;
    void set_vec3(const std::string& name, glm::vec3 value) const;
    void set_mat4(const std::string& name, glm::mat4 value) const;

  private:
    // function for checking shader compilation/linking errors.
    void check_errors(unsigned int shader, std::string type);
};

#endif
