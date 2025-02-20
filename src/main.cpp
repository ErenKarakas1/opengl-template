#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "math.hpp"
#include "color.hpp"
#include "shader.hpp"

#include <array>
#include <iostream>

using Vec3 = utils::math::Vec3;
using float4 = utils::color::float4;

namespace {
void framebufferSizeCallback(GLFWwindow *window, const int width, const int height) {
    (void) window;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}
} // namespace

struct Vertex {
    Vec3 position{};
    float4 color{};
};

int main() {
    constexpr unsigned int HEIGHT = 600;
    constexpr unsigned int WIDTH = 800;

    constexpr std::array<Vertex, 3> vertices = {
        Vertex{{0.5F, -0.5F, 0.0F},  normalize_color(utils::color::RED)},
        Vertex{{-0.5F, -0.5F, 0.0F}, normalize_color(utils::color::GREEN)},
        Vertex{{0.0F, 0.5F, 0.0F},   normalize_color(utils::color::BLUE)}
    };

    if (glfwInit() == 0) {
        std::cerr << "Failed to initialize GLFW" << '\n';
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SCALE_FRAMEBUFFER, GLFW_FALSE);             // Wayland-specific

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Hello, World", nullptr, nullptr);

    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << '\n';
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0) {
        std::cerr << "Failed to initialize GLAD" << '\n';
        glfwTerminate();
        return -1;
    }

    const GLuint program = shaders::create_shader_program("shaders/vertex.glsl", "shaders/fragment.glsl");

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(sizeof(vertices[0].position)));
    glEnableVertexAttribArray(1);

    while (glfwWindowShouldClose(window) == 0) {
        processInput(window);

        glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(program);

    glfwTerminate();
    return 0;
}
