#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>
#include <string>

class Shader {
  public:
    Shader(const char* vertex_path, const char* fragment_path);
    ~Shader();
    unsigned int get_program_id();

  private:
    const std::string read_file(const char* file_path);
    unsigned int program_id_;
};

#endif
