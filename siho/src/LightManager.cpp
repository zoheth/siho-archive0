#include <siho/LightManager.h>

#include <siho/UniformObject.h>

void LightManager::AddPointLight(const PointLight light)
{
    point_lights_.push_back(light);
}

void LightManager::SetUniforms(UniformObject& uniform_obj) const {
	const int point_light_count = static_cast<int>(point_lights_.size());
    uniform_obj.set(uniforms::kPointLightsCount, point_light_count);
    for (int i = 0; i < point_light_count; ++i) {
        glm::vec3 position = point_lights_[i].position;
        float cos_a = cos(point_lights_[i].angle_y);
        float sin_a = sin(point_lights_[i].angle_y);
        position.x = point_lights_[i].position.x * cos_a + point_lights_[i].position.z * sin_a;
        position.z = -point_lights_[i].position.x * sin_a + point_lights_[i].position.z * cos_a;
        uniform_obj.set(uniforms::PointLight(i,uniforms::Light::POSITION), position);
        uniform_obj.set(uniforms::PointLight(i, uniforms::Light::COLOR), point_lights_[i].color * point_lights_[i].intensity);
        uniform_obj.set(uniforms::PointLight(i, uniforms::Light::INTENSITY), point_lights_[i].intensity);
    }

    const int spot_light_count = static_cast<int>(spot_lights_.size());
    uniform_obj.set(uniforms::kSpotLightsCount, spot_light_count);
    for(int i = 0; i < spot_light_count; ++i)
    {
    	uniform_obj.set(uniforms::SpotLight(i, uniforms::Light::POSITION), spot_lights_[i].position);
		uniform_obj.set(uniforms::SpotLight(i, uniforms::Light::DIRECTION), spot_lights_[i].direction);
		uniform_obj.set(uniforms::SpotLight(i, uniforms::Light::COLOR), spot_lights_[i].color);
		uniform_obj.set(uniforms::SpotLight(i, uniforms::Light::INTENSITY), spot_lights_[i].intensity);
		uniform_obj.set(uniforms::SpotLight(i, uniforms::Light::CUTOFF), spot_lights_[i].cutoff);
		uniform_obj.set(uniforms::SpotLight(i, uniforms::Light::OUTER_CUTOFF), spot_lights_[i].outer_cutoff);
	}
}
