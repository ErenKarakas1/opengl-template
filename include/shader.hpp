#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace shaders {

inline std::string read_file(const std::string_view file_path) {
    std::ifstream file(file_path.data(), std::ios::in);
    if (!file) {
        std::cerr << "Error: Could not open shader file " << file_path << "\n";
        assert(false && "Shader file not found");
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

inline GLuint compile_shader(const GLenum shader_type, const std::string& source) {
    const GLuint shader = glCreateShader(shader_type);

    const char* source_cstr = source.c_str();
    glShaderSource(shader, 1, &source_cstr, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint log_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

        std::string log(log_length, ' ');
        glGetShaderInfoLog(shader, log_length, &log_length, log.data());

        std::cerr << "Error compiling shader: " << log << "\n";
        assert(false && "Shader compilation failed");
    }
    return shader;
}

inline GLuint create_shader_program(const std::string_view vertex_shader_path,
                                    const std::string_view fragment_shader_path) {
    const std::string vertexSource = read_file(vertex_shader_path);
    const std::string fragmentSource = read_file(fragment_shader_path);

    const GLuint vertexShader = compile_shader(GL_VERTEX_SHADER, vertexSource);
    const GLuint fragmentShader = compile_shader(GL_FRAGMENT_SHADER, fragmentSource);

    const GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint log_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

        std::string log(log_length, ' ');
        glGetProgramInfoLog(program, log_length, &log_length, log.data());

        std::cerr << "Error linking shader program: " << log << "\n";
        assert(false && "Shader linking failed");
    }

    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

} // namespace shaders

#endif // SHADER_HPP
