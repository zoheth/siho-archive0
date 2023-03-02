#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "UniformObject.h"

class PointLight {
public:
    glm::vec3 position_;
    glm::vec3 ambient_;
    glm::vec3 diffuse_;
    glm::vec3 specular_;
    float constant_;
    float linear_;
    float quadratic_;
};

class DirectionalLight {
public:
    glm::vec3 direction_;
    glm::vec3 ambient_;
    glm::vec3 diffuse_;
    glm::vec3 specular_;
};

class SpotLight {
public:
    glm::vec3 position_;
    glm::vec3 direction_;
    glm::vec3 ambient_;
    glm::vec3 diffuse_;
    glm::vec3 specular_;
    float constant_;
    float linear_;
    float quadratic_;
    float cutoff_;
    float outer_cutoff_;
};

class LightManager {
public:
    void addPointLight(PointLight light);
    UniformObject GetLightData() const;



private:
    DirectionalLight m_directionalLight;
    std::vector<PointLight> m_pointLights;
    SpotLight m_spotLight;

};
