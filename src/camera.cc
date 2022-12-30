#include "camera.h"

Camera::Camera(glm::vec3 pos, glm::vec3 up)
    : front_(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED), mouse_sensitivity(SENSITIVITY),
      fov(FOV), yaw_(YAW), pitch_(PITCH), position(pos), world_up_(up) {
    update_camera();
}

glm::mat4 Camera::get_view_matrix() { return glm::lookAt(position, position + front_, up_); }

void Camera::update_camera() {
    using namespace glm;

    // calculate the new front vector
    vec3 front;
    front.x = cos(radians(yaw_)) * cos(radians(pitch_));
    front.y = sin(radians(pitch_));
    front.z = sin(radians(yaw_)) * cos(radians(pitch_));
    front_ = normalize(front);

    // also re-calculate the right and up vector
    right_ = normalize(cross(front_, world_up_));
    up_ = normalize(cross(right_, front_));
}

void Camera::process_keyboard(Movement direction, float deltaTime) {
    float velocity = movement_speed * deltaTime;
    if (direction == Movement::FORWARD)
        position += front_ * velocity;
    if (direction == Movement::BACKWARD)
        position -= front_ * velocity;
    if (direction == Movement::LEFT)
        position -= right_ * velocity;
    if (direction == Movement::RIGHT)
        position += right_ * velocity;
}

void Camera::process_mouse(float xoffset, float yoffset) {
    xoffset *= mouse_sensitivity;
    yoffset *= mouse_sensitivity;

    yaw_ += xoffset;
    pitch_ += yoffset;

    if (pitch_ > 89.0f)
        pitch_ = 89.0f;
    if (pitch_ < -89.0f)
        pitch_ = -89.0f;

    update_camera();
}
