#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"

#include "camera.h"
#include "program.h"
#include "shader.h"

#include <iostream>

Camera cam_ = Camera(glm::vec3(0.f, 0.f, 10.f));
float delta_time = 0.0f;
float last_frame = 0.0f;

glm::vec3 cube_positions[] = {
    glm::vec3(-5.f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(5.0f, 0.0f, 0.0f),
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
    glfwSetCursorPosCallback(window_, mouse_callback);
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewInit();

    // Initialize VBO
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Initialize VAO
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Program::run() {
    // GL parameters
    glEnable(GL_DEPTH_TEST);

    // Create shader
    Shader shader("../src/shaders/default.vert", "../src/shaders/default.frag");

    // Load and create a texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image, create texture and generate mipmaps
    int width, height, channels;
    unsigned char* data = stbi_load("../res/container.jpg", &width, &height, &channels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    shader.use();
    shader.set_int("texture", 0);

    // Set up projection matrix
    glm::mat4 projection =
        glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
    shader.set_mat4("projection", projection);

    // Set up view matrix
    glm::mat4 view = cam_.get_view_matrix();
    shader.set_mat4("view", view);

    // Set up model matrix
    glm::mat4 model = glm::mat4(1.0f);
    shader.set_mat4("model", model);

    // time variables for cube translation
    float time = 0.0f;
    float velocity = 2.f;
    float amplitude = 0.05f;

    // Main loop
    while (!glfwWindowShouldClose(window_)) {
        // time calculations
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        time += delta_time;

        // input
        process_input(window_);

        // clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update cube positions of first and second cube
        cube_positions[0].x += amplitude * sin(velocity * time);
        cube_positions[1].y -= amplitude * sin(velocity * time);

        glBindVertexArray(vao_);
        glBindTexture(GL_TEXTURE_2D, texture);

        // render 3 cubes
        for (unsigned i = 0; i < 3; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cube_positions[i]);
            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.set_mat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // glfw swap buffers and poll IO events
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }

    glfwTerminate();
}

// Cleanup
Program::~Program() { glfwTerminate(); }

void Program::process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam_.process_kb(FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam_.process_kb(BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam_.process_kb(LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam_.process_kb(RIGHT, delta_time);
}

void Program::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (first_mouse_) {

        last_x_ = xpos;
        last_y_ = ypos;
        first_mouse_ = false;
    }

    float xoffset = xpos - last_x_;
    float yoffset = last_y_ - ypos; // reverse y axis

    last_x_ = xpos;
    last_y_ = ypos;

    cam_.process_mouse(xoffset, yoffset);
}

void Program::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
