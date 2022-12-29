#include "program.h"
#include "camera.h"
#include "model.h"
#include "shader.h"

#include <iostream>

const double FPS_LIMIT = 200;

Camera cam_ = Camera(glm::vec3(0.f, 0.f, 10.f));
double delta_time = 0.0f;
double last_frame = 0.0f;

// Setup window and OpenGL context
Program::Program() {
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // unlimit fps
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);

    window_ = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window_);
    glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
    glfwSetCursorPosCallback(window_, mouse_callback);
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewInit();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void Program::run() {
    // GL parameters
    glEnable(GL_DEPTH_TEST);

    // Create shader and activate it
    Shader shader("../src/shaders/default.vert", "../src/shaders/default.frag");
    shader.use();

    // Create model
    Model scene("../assets/scene/scene.obj");

    for (auto i : scene.textures_loaded) {
        std::cout << "texture loaded: " << i.type << std::endl;
    }

    // setup projection matrix
    glm::mat4 projection =
        glm::perspective(glm::radians(cam_.zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.f);
    shader.set_mat4("projection", projection);

    // Main loop
    while (!glfwWindowShouldClose(window_)) {
        // time calculations
        double current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;

        glfwPollEvents();

        if ((current_frame - last_frame) >= 1.0 / FPS_LIMIT) {
            // input
            process_input(window_);

            // clear screen
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // setup camera and projection
            glm::mat4 view = cam_.get_view_matrix();
            shader.set_mat4("view", view);

            // render model
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f)); // center the model
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));  // scale the model
            shader.set_mat4("model", model);
            scene.draw(shader);

            // glfw swap buffers and poll IO events
            glfwSwapBuffers(window_);

            last_frame = current_frame;
        }
    }

    glfwTerminate();
}

// Cleanup
Program::~Program() { glfwTerminate(); }

// callback function for input events
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
