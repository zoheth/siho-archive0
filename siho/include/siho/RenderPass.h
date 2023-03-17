#pragma once
#include "RenderTarget.h"
#include "Shader.h"
#include "IBLRenderer.h"

class Scene;
class Camera;

class RenderPass {
public:
	virtual ~RenderPass() = default;
	RenderPass(RenderTarget* target, Shader& shader) : shader_(shader), target_(target) {}
    virtual void render() = 0;

	void Enable() { enable_ = true; }
	void Disable() { enable_ = false; }

    [[nodiscard]] Shader& shader() const { return shader_; }
    [[nodiscard]] RenderTarget* target() const { return target_; }
protected:
    Shader& shader_;
    RenderTarget* target_=nullptr;
	bool enable_ = true;
};


class FinalRenderPass final : public RenderPass
{
	public:
	FinalRenderPass(RenderTarget* target, Shader& shader, RenderPass* prev_render_pass)
		: RenderPass(target, shader), prev_render_pass_(prev_render_pass)
	{
	}
	void set_prev_pass(RenderPass* prev_render_pass) { prev_render_pass_ = prev_render_pass; }

	void render() override;
private:
	RenderPass* prev_render_pass_;
};


class BlendPass final : public RenderPass
{
public:
	BlendPass(RenderTarget* target, Shader& shader)
		: RenderPass(target, shader)
	{
	}

	void render() override;
	void AddTexture(const unsigned int texture_id) { blend_texture_ids_.push_back(texture_id); }
private:
	std::vector<unsigned int> blend_texture_ids_;
};


class ShadowMapPass final : public RenderPass {
public:
	ShadowMapPass(RenderTarget* target, Shader& shader, Scene& scene)
		: RenderPass(target, shader), scene_(scene)
	{
	}

    void render() override;
private:
	Scene& scene_;
};


class SceneRenderPass final : public RenderPass {
public:
	SceneRenderPass(RenderTarget* target, Shader& shader, Scene& scene)
		: RenderPass(target, shader), scene_(scene)
	{
	}

    void render() override;
	void SetShadowMapPass(ShadowMapPass* shadow_map_pass) { shadow_map_pass_ = shadow_map_pass; }
	void SetIblRenderer(IBLRenderer* ibl) { ibl_ = ibl; }

private:
	Scene& scene_;
	IBLRenderer* ibl_=nullptr;
    ShadowMapPass* shadow_map_pass_=nullptr;
};


class BlurPass final : public RenderPass
{
public:
	BlurPass(RenderTarget* target, Shader& shader, SceneRenderPass& scene_render_pass)
		: RenderPass(target, shader), scene_render_pass_(scene_render_pass)
	{
		other_target_ = new RenderTarget(target->width(), target->height(), 1, 1);
	}
	void render() override;
	[[nodiscard]] RenderTarget* scene_render_target() const { return scene_render_pass_.target(); }
private:
	SceneRenderPass& scene_render_pass_;
	RenderTarget* other_target_;
};


class BloomPass final : public RenderPass
{
public:
	BloomPass(RenderTarget* target, Shader& shader, BlurPass& blur_pass)
		: RenderPass(target, shader), blur_pass_(blur_pass)
	{
	}

	void render() override;
private:
	BlurPass& blur_pass_;
};