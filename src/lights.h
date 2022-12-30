#ifndef LIGHTS_H
#define LIGHTS_H

#include <glm/glm.hpp>

struct DirectionalLight {
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    DirectionalLight(glm::vec3 dir) : direction(dir) {
        using namespace glm;

        ambient = vec3(0.30f, 0.30f, 0.35f);
        diffuse = vec3(0.18f, 0.18f, 0.18f);
        specular = vec3(1.0f);
    }
};

struct PointLight {
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant, linear, quadratic;

    PointLight(glm::vec3 pos) : position(pos) {
        using namespace glm;

        position = vec3(2.0f, 5.0f, 0.0f);
        ambient = vec3(0.2f);
        diffuse = vec3(0.5f);
        specular = vec3(1.f);

        constant = 1.0f;
        linear = 0.1f;
        quadratic = 0.032f;
    }
};

#endif
