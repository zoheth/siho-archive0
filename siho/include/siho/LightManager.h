#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "UniformObject.h"

struct PointLight {
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
};

struct  DirectionalLight {
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
};

struct SpotLight {
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
    float cutoff;
    float outer_cutoff;
};

class LightManager {
public:

    void AddPointLight(PointLight light);
    void setDirectionalLight(DirectionalLight light);
    void AddSpotLight(SpotLight light);


    void SetUniforms(UniformObject& uniform_obj) const;

private:
    DirectionalLight directional_light_{};
    std::vector<PointLight> point_lights_{};
    std::vector<SpotLight> spot_lights_{};
};
