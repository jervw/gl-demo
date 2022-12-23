#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>
#include <string>

class Shader {
  public:
    Shader(const char* vertex_path, const char* fragment_path);
    ~Shader();
    GLuint get_program_id();

  private:
    std::string read_file(const char* file_path);
    GLuint compile_shader(const char* shader_code, GLenum shader_type);

    GLuint program_id_;
};

#endif
