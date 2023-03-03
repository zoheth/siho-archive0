#include <siho/RenderPass.h>
#include <glad/glad.h>

#include "siho/Scene.h"


void ShadowMapPass::render(Scene& scene)
{
	target_->bind();
	glClear(GL_DEPTH_BUFFER_BIT);
	shader_->use();

	for (auto& renderable: scene.renderables())
	{
		//renderable.render();
	}
	RenderTarget::unbind();
}

void ShadowMapPass::setRenderTarget(RenderTarget* target)
{
	target_ = target;
}

void ShadowMapPass::addDependency(RenderPass* pass)
{
}

void FinalRenderPass::render()
{
	target_->bind();
	shader_->use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadow_map_pass_->target()->depth_stencil_buffer());
	RenderTarget::unbind();
}

