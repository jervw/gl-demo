#include <fstream>
#include <iostream>
#include <vector>

#include "shader.h"

Shader::Shader(const char* vertex_path, const char* fragment_path) {

    // Create the shaders
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    // Load the shader source code from file
    std::string vertex_shader_code = read_file(vertex_path);
    std::string fragment_shader_code = read_file(fragment_path);

    // Compile the shader
    compile_shader(vertex_shader_code.c_str(), vertex_shader_id);
    compile_shader(fragment_shader_code.c_str(), fragment_shader_id);

    // Link the program
    program_id_ = glCreateProgram();
    glAttachShader(program_id_, vertex_shader_id);
    glAttachShader(program_id_, fragment_shader_id);
    glLinkProgram(program_id_);

    // Check the program for errors
    GLint success;
    GLchar info_log[512];
    glGetProgramiv(program_id_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program_id_, 512, NULL, info_log);
        std::cout << "Shader program link error: " << info_log << std::endl;
    }

    // Detach and delete the shaders as they're linked into our program now and no longer necessary
    glDetachShader(program_id_, vertex_shader_id);
    glDetachShader(program_id_, fragment_shader_id);

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
}

Shader::~Shader() { glDeleteProgram(program_id_); }

GLuint Shader::get_program_id() { return program_id_; }

// load the shader source code from a file
std::string Shader::read_file(const char* file_path) {
    std::string content;
    std::ifstream file_stream(file_path, std::ios::in);

    if (!file_stream.is_open()) {
        std::cout << "Could not read file " << file_path << ". File does not exist." << std::endl;
        getchar();
        return "";
    }

    std::string line = "";
    while (!file_stream.eof()) {
        std::getline(file_stream, line);
        content.append(line + "");
    }

    file_stream.close();
    return content;
}

// compile the shader source code
GLuint Shader::compile_shader(const char* shader_code, GLenum shader_type) {
    printf("Compiling shader type %d \n", shader_type);

    GLuint shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, &shader_code, NULL);
    glCompileShader(shader_id);

    // check for shader for compile errors
    GLint result = GL_FALSE;
    int info_log_length;

    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0) {
        std::vector<char> shader_error_message(info_log_length + 1);
        glGetShaderInfoLog(shader_id, info_log_length, NULL, &shader_error_message[0]);
        printf("%s\n", &shader_error_message[0]);
    }

    return shader_id;
}
