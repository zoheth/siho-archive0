#pragma once
#include <vector>

#include "LightManager.h"
#include "Renderable.h"
#include "Shader.h"
#include "UniformObject.h"

class Scene
{
public:

	void AddRenderable(const Renderable& renderable) { renderables_.push_back(renderable); }
    void AddLight(const PointLight& light) { light_manager_.AddPointLight(light); }

    void render(Shader& shader) {
        // 设置光源uniform
        light_manager_.SetUniforms(uniform_obj_);

        for (auto& renderable : renderables_) {

            renderable.render(shader, uniform_obj_);
        }
    }
    void set_renderables(std::vector<Renderable>&& renderables) { renderables_ = std::move(renderables); }

    std::vector<Renderable>& renderables() { return renderables_; }
private:
    UniformObject uniform_obj_;
	LightManager light_manager_{};
	std::vector<Renderable> renderables_{};
};

