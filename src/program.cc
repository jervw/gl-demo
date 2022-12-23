#include "program.h"
#include "mesh.h"
#include "shader.h"

// Vertex data
const GLfloat vertex_data[] = {
    -1.0f, -1.f, 0.f, 1., -1.0, 0.f, 0.f, 1.f, 0.f,
};

// Setup window and OpenGL context
Program::Program() {
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window_);
    glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);

    glewInit();

    // Initialize VBO
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

    // Initialize VAO
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void Program::run() {
    // load shaders which are located in src/shaders
    Shader simple_shader("../src/shaders/simple.vert", "../src/shaders/simple.frag");

    // create cube mesh
    //Mesh cube("../res/cube.obj", simple_shader);

    // Main loop
    while (!glfwWindowShouldClose(window_)) {

        // Input
        process_input(window_);

        // Render
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw cube
        // cube.draw();


        // glfw swap buffers and poll IO events
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}

// Cleanup
Program::~Program() { glfwTerminate(); }

void Program::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Program::process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
