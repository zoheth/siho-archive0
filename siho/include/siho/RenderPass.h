#pragma once
#include "RenderTarget.h"
#include "Shader.h"

class Scene;
class Camera;

class RenderPass {
public:
	virtual ~RenderPass() = default;
	RenderPass(RenderTarget* target, Shader* shader) : shader_(shader), target_(target) {}
    virtual void render() = 0;
    virtual void render(Scene& scene) = 0;
    virtual void setRenderTarget(RenderTarget* target) = 0;
    virtual void addDependency(RenderPass* pass) = 0;
    [[nodiscard]] Shader* shader() const { return shader_; }
    [[nodiscard]] RenderTarget* target() const { return target_; }
protected:
    Shader* shader_;
    RenderTarget* target_;
};

class ShadowMapPass : public RenderPass {
public:
	ShadowMapPass(RenderTarget* target, Shader* shader)
		: RenderPass(target, shader)
	{
	}

    void render(Scene& scene) override;

    void setRenderTarget(RenderTarget* target) override;

    void addDependency(RenderPass* pass) override;
};

class FinalRenderPass : public RenderPass {
public:
	FinalRenderPass(RenderTarget* target, Shader* shader)
		: RenderPass(target, shader)
	{
	}

    void render() override;

    void setRenderTarget(RenderTarget* target) override;

    void addDependency(RenderPass* pass) override;
private:
    ShadowMapPass* shadow_map_pass_=nullptr;
};