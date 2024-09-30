#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <array>
#include <filesystem>

class Shader {
public:
    unsigned int ID;

    Shader(const std::filesystem::path &vertexPath, const std::filesystem::path &fragmentPath) {
        std::string vertShaderCode = readFile(vertexPath);
        std::string fragShaderCode = readFile(fragmentPath);

        unsigned int vertex;
        unsigned int fragment;

        if (!compileShader(&vertex, GL_VERTEX_SHADER, vertShaderCode)) {
            std::cerr << "Vertex shader compilation failed" << std::endl;
        }

        if (!compileShader(&fragment, GL_FRAGMENT_SHADER, fragShaderCode)) {
            std::cerr << "Fragment shader compilation failed" << std::endl;
        }

        if (!linkProgram(vertex, fragment)) {
            std::cerr << "Shader program linking failed" << std::endl;
        }
    }

    void use() const {
        glUseProgram(ID);
    }

    void setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
    }

    void setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

private:
    [[nodiscard]] std::string readFile(const std::filesystem::path &filePath) const {
        std::string shaderCode;
        std::ifstream shaderFile;

        shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            shaderFile.open(filePath);

            std::stringstream shaderStream;
            shaderStream << shaderFile.rdbuf();

            shaderFile.close();
            shaderCode = shaderStream.str();
        } catch (std::ifstream::failure &e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        return shaderCode;
    };

    bool compileShader(unsigned int *shader, unsigned int type, const std::string_view source) {
        *shader = glCreateShader(type);

        const char* const sourceData = source.data();

        glShaderSource(*shader, 1, &sourceData, nullptr);
        glCompileShader(*shader);

        int success;

        std::array<char, m_logSize> infoLog;

        glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(*shader, m_logSize, nullptr, infoLog.data());
            std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog.data() << std::endl;
        }

        return success;
    }

    bool linkProgram(unsigned int vertex, unsigned int fragment) {
        ID = glCreateProgram();

        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        
        glLinkProgram(ID);

        int success;

        std::array<char, m_logSize> infoLog;

        glGetProgramiv(ID, GL_LINK_STATUS, &success);

        if (!success) {
            glGetProgramInfoLog(ID, m_logSize, nullptr, infoLog.data());
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog.data() << std::endl;
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);

        return success;
    }

    constexpr static int m_logSize = 512;
};

#endif // SHADER_HPP
