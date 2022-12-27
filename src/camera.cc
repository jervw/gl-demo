#include "camera.h"

#include <iostream>

// Camera values

Camera::Camera(glm::vec3 pos) {
    position = pos;
    world_up = glm::vec3(0.0f, 1.0f, 0.0f);
    yaw = -90.0f;
    pitch = 0.0f;
    movement_speed = 2.5f;
    mouse_sensitivity = 0.1f;
    zoom = 45.0f;
    update_camera();
}

glm::mat4 Camera::get_view_matrix() { return glm::lookAt(position, position + front, up); }

void Camera::process_keyboard(Movement direction, float delta_time) {
    float velocity = movement_speed * delta_time;
    if (direction == FORWARD) {
        position += front * velocity;
    }
    if (direction == BACKWARD) {
        position -= front * velocity;
    }
    if (direction == LEFT) {
        position -= right * velocity;
    }
    if (direction == RIGHT) {
        position += right * velocity;
    }
}

void Camera::process_mouse_movement(float xoffset, float yoffset) {
    std::cout << "xoffset: " << xoffset << " yoffset: " << yoffset << std::endl;


    xoffset *= mouse_sensitivity;
    yoffset *= mouse_sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    // update vectors using the updated Euler angles
    update_camera();
}

void Camera::update_camera() {
    // calculate the new front vector
    glm::vec3 front;

    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(front);
    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
}
