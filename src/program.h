#ifndef PROGRAM_H
#define PROGRAM_H

#include "camera.h"
#include "lights.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class Program {
  public:
    Program();
    ~Program();

    void run();

  private:
    // callback functions
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void process_input(GLFWwindow* window);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

    GLFWwindow* window_;

    // camera
    static inline Camera cam_ = Camera(glm::vec3(0.f, 2.f, 8.f));
    static inline DirectionalLight light = DirectionalLight(glm::vec3(0.f, 0.f, -1.f));
};

#endif
