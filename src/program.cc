#include "program.h"
#include "model.h"
#include "shader.h"

#include <glm/ext/matrix_transform.hpp>
#include <iostream>

// window properties
const unsigned WIDTH = 1280, HEIGHT = 720;
const double FPS_LIMIT = 120.0;

// timing
double delta_time = 0.0f;
double last_frame = 0.0f;

// mouse movement
float last_x = WIDTH / 2.f;
float last_y = HEIGHT / 2.f;
bool first_mouse = true;

// Setup window and OpenGL context
Program::Program() {
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE); // unlimit fps

    window_ = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Demo", nullptr, nullptr);
    glfwMakeContextCurrent(window_);
    glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
    glfwSetCursorPosCallback(window_, mouse_callback);
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewInit();
}

// Cleanup
Program::~Program() { glfwTerminate(); }

void Program::run() {
    // GL parameters
    glEnable(GL_DEPTH_TEST);

    // Create shaders and activate it
    Shader scene_shader("../shaders/default.vert", "../shaders/default.frag");

    scene_shader.use();

    // Create scene model
    Model scene("../assets/scene.obj");

    // setup directional light
    scene_shader.set_vec3("dirLight.diffuse", light.diffuse);
    scene_shader.set_vec3("dirLight.specular", light.specular);

    // setup camera projection matrix
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection =
        glm::perspective(glm::radians(cam_.fov), (float)WIDTH / (float)HEIGHT, 0.1f, 100.f);
    scene_shader.set_mat4("projection", projection);

    // setup day and night cycle
    glm::vec3 ambient_cycle = glm::vec3(0.0f);
    double ambient_cycle_factor = 4.0;
    glm::vec3 min_light = glm::vec3(0.15f);
    glm::vec3 max_light = glm::vec3(0.40f);

    // Main loop
    while (!glfwWindowShouldClose(window_)) {
        // time calculations
        double current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        float day_cycle = sin(current_frame / ambient_cycle_factor) / 2.f + 0.5f;

        glfwPollEvents();

        if ((current_frame - last_frame) >= 1.0 / FPS_LIMIT) {
            // input
            process_input(window_);

            // clear screen
            glClearColor(0.53f, 0.8f, 0.92f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // update camera and projection
            view = cam_.get_view_matrix();
            scene_shader.set_mat4("view", view);

            // animate day and night cycle
            light.ambient = glm::mix(min_light, max_light, day_cycle);

            // rotate directional light
            light.direction.x = sin(current_frame / 10.0);
            light.direction.y = cos(current_frame / 10.0);

            // update directional light
            scene_shader.set_vec3("dirLight.direction", light.direction);
            scene_shader.set_vec3("dirLight.ambient", light.ambient);

            // render scene model
            glm::mat4 scene_model = glm::mat4(1.0f);
            scene_shader.set_mat4("model", scene_model);
            scene_shader.set_vec3("viewPos", cam_.position);
            scene.draw(scene_shader);

            // glfw swap buffers and poll IO events
            glfwSwapBuffers(window_);

            last_frame = current_frame;
        }
    }

    glfwTerminate();
}

// callback function for input events
void Program::process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam_.process_keyboard(Movement::FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam_.process_keyboard(Movement::BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam_.process_keyboard(Movement::LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam_.process_keyboard(Movement::RIGHT, delta_time);

    // temporary light source movement
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        light.direction.z -= 0.1f * delta_time;

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        light.direction.z += 0.1f * delta_time;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        light.direction.x -= 0.1f * delta_time;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        light.direction.x += 0.1f * delta_time;
}

void Program::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (first_mouse) {

        last_x = xpos;
        last_y = ypos;
        first_mouse = false;
    }

    float xoffset = xpos - last_x;
    float yoffset = last_y - ypos; // reverse y axis

    last_x = xpos;
    last_y = ypos;

    cam_.process_mouse(xoffset, yoffset);
}

void Program::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
