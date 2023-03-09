#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "UniformObject.h"

struct PointLight {
    glm::vec3 position;
    glm::vec3 color;
    float intensity = 250;

    float angle_y;

    void SetPosition(const glm::vec3& position) { this->position = position; }
    void SetColor(const glm::vec3& color) { this->color = color; }
    void RotateY(const float angle) {
	    float cosA = cos(angle);
	    float sinA = sin(angle);
        float x = position.x * cosA + position.z * sinA;
        float z = -position.x * sinA + position.z * cosA;
        position.x = x;
        position.z = z;
        angle_y += angle;
    }
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

    [[nodiscard]] int GetPointLightCount() const { return point_lights_.size(); }
	[[nodiscard]] int GetSpotLightCount() const { return spot_lights_.size(); }


    void SetUniforms(UniformObject& uniform_obj) const;

private:
    friend class Scene;
    DirectionalLight directional_light_{};
    std::vector<PointLight> point_lights_{};
    std::vector<SpotLight> spot_lights_{};
};
