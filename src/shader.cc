#include <fstream>
#include <iostream>
#include <vector>

#include "shader.h"
#include <sstream>

Shader::Shader(const char* vertex_path, const char* fragment_path) {
    // Vertex shader
    unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    const std::string vertex_code = read_file(vertex_path);
    const char* vertex_code_c = vertex_code.c_str();
    glShaderSource(vertex_shader_id, 1, &vertex_code_c, nullptr);
    glCompileShader(vertex_shader_id);

    // Fragment shader
    unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    const std::string fragment_code = read_file(fragment_path);
    const char* fragment_code_c = fragment_code.c_str();
    glShaderSource(fragment_shader_id, 1, &fragment_code_c, nullptr);
    glCompileShader(fragment_shader_id);

    // check for shader compile errors
    int success;
    char info_log[512];

    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader_id, 512, nullptr, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED " << info_log << std::endl;
    }

    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader_id, 512, nullptr, info_log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED " << info_log << std::endl;
    }

    // Link shaders
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader_id);
    glAttachShader(shader_program, fragment_shader_id);
    glLinkProgram(shader_program);

    // Check for linking errors
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED " << info_log << std::endl;
    }

    // Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
}

Shader::~Shader() { glDeleteProgram(program_id_); }

unsigned int Shader::get_program_id() { return program_id_; }

// load the shader source code from a file

const std::string Shader::read_file(const char* file_path) {
    std::string shader_code;
    std::ifstream shader_file;
    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // open file
        shader_file.open(file_path);
        std::stringstream shader_stream;

        // read file's buffer content into stream
        shader_stream << shader_file.rdbuf();

        // close file handler
        shader_file.close();

        // convert stream into string
        shader_code = shader_stream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    return shader_code;
}
