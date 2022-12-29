#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum class Movement { FORWARD, BACKWARD, LEFT, RIGHT };

// default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;

class Camera {
  public:
    Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = YAW, float pitch = PITCH);

    // camera options
    float movement_speed;
    float mouse_sensitivity;
    float fov;

    // returns the view matrix
    glm::mat4 get_view_matrix();

    void process_keyboard(Movement direction, float deltaTime);
    void process_mouse(float xoffset, float yoffset);

  private:
    // camera Attributes
    glm::vec3 position_, front_, up_, right_, world_up_;

    // euler Angles
    float yaw_, pitch_;

    // calculates the front vector from new yaw and pitch values
    void update_camera();
};
#endif
