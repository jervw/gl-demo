#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum Movement { FORWARD, BACKWARD, LEFT, RIGHT };

class Camera {
  public:
    // camera attributes
    glm::vec3 position, front, up, right, world_up;
    float movement_speed, mouse_sensitivity, zoom;
    float yaw, pitch;

    Camera(glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f));

    glm::mat4 get_view_matrix();

    void process_keyboard(Movement direction, float delta_time);
    void process_mouse_movement(float xoffset, float yoffset);

  private:
    // update vectors using the updated Euler angles
    void update_camera();
};

#endif
