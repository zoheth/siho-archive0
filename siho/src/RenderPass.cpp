#include <siho/RenderPass.h>
#include <glad/glad.h>

#include "siho/Scene.h"

void RenderQuad();

void FinalRenderPass::render()
{
	prev_render_pass_.render();
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	shader_.use();
	shader_.setInt(uniforms::kScreenTexture, 0);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, prev_render_pass_.target()->color_buffer());
	RenderQuad();
	glEnable(GL_DEPTH_TEST);
}

void BlendPass::render()
{
	target_->bind();
	glClear(GL_COLOR_BUFFER_BIT);
	shader_.use();
	glDisable(GL_DEPTH_TEST);
	for (unsigned int i = 0; i < blend_texture_ids_.size(); i++)
	{
		shader_.setInt(uniforms::kScreenTexture + std::to_string(i), i);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, blend_texture_ids_[i]);
	}
	RenderQuad();
	glEnable(GL_DEPTH_TEST);
	RenderTarget::unbind();
}

void ShadowMapPass::render()
{
	target_->bind();
	glClear(GL_DEPTH_BUFFER_BIT);
	shader_.use();

	scene_.render(shader_);
	RenderTarget::unbind();
}


void SceneRenderPass::render()
{
	if(shadow_map_pass_)
	{
		shadow_map_pass_->render();
	}

	target_->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	shader_.use();

	if(shadow_map_pass_)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shadow_map_pass_->target()->depth_stencil_buffer());
	}

	if(ibl_)
	{
		glActiveTexture(GL_TEXTURE6);
		shader_.setInt("u_IrradianceMap", 6);
		glBindTexture(GL_TEXTURE_CUBE_MAP, ibl_->irradiance_map());
	}

	scene_.render(shader_);

	if(ibl_)
	{
		ibl_->RenderSkybox();
	}
	RenderTarget::unbind();
}

void BlurPass::render()
{
	scene_render_pass_.render();

	bool first_iteration = true;
	constexpr int amount = 10;
	for (unsigned int i = 0; i < amount; i++)
	{
		target_->bind();
		shader_.use();
		shader_.setInt(uniforms::kScreenTexture, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader_.setBool("u_Horizontal", true);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, first_iteration ? scene_render_pass_.target()->color_buffer(1) : other_target_->color_buffer());
		glDisable(GL_DEPTH_TEST);
		RenderQuad();
		glEnable(GL_DEPTH_TEST);
		if (first_iteration)
			first_iteration = false;
		

		other_target_->bind();
		shader_.use();
		shader_.setInt(uniforms::kScreenTexture, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader_.setBool("u_Horizontal", false);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, target_->color_buffer());
		RenderQuad();
	}

	RenderTarget::unbind();
	
}

void BloomPass::render()
{
	blur_pass_.render();

	target_->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader_.use();
	shader_.setInt(uniforms::kScreenTexture + "0", 0);
	shader_.setInt(uniforms::kScreenTexture + "1", 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, blur_pass_.scene_render_target()->color_buffer());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, blur_pass_.target()->color_buffer());

	shader_.setInt("u_InputTexture", 0);
	shader_.setInt("u_BlurTexture", 1);

	RenderQuad();

	RenderTarget::unbind();
}

unsigned int quad_vao=0;
void RenderQuad()
{
	if(quad_vao==0)
	{
		constexpr float quad_vertices[] = {
		-1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		 1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 1.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		};
		unsigned int quad_vbo;
		glGenVertexArrays(1, &quad_vao);
		glGenBuffers(1, &quad_vbo);
		glBindVertexArray(quad_vao);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void*>(0));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	}
	else
	{
		glBindVertexArray(quad_vao);
	}
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	//glDeleteVertexArrays(1, &quad_vao);
	//glDeleteBuffers(1, &quad_vbo);
}