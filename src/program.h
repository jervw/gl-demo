#ifndef PROGRAM_H
#define PROGRAM_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Program {
  public:
    Program();
    ~Program();
    void run();

  private:
    static constexpr unsigned WIDTH = 1280;
    static constexpr unsigned HEIGHT = 720;

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void process_input(GLFWwindow* window);

    GLFWwindow* window_;
    GLuint vbo_;
    GLuint vao_;
};

#endif
