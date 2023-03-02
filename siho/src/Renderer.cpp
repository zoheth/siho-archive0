#include <siho/Renderer.h>

#include <siho/Scene.h>
#include "siho/MaterialInstance.h"

#include <glad/glad.h>

using namespace siho;


Renderer::Renderer(Engine& engine):
	mEngine(engine)
{
}

void Renderer::prepare()
{
	
}

void Renderer::render(View const* view)
{
	const Scene& scene = *view->getScene();
	Camera& camera = *view->getCullingCamera();
	std::vector<MaterialInstance::Parameter> context_params;
	context_params.push_back({ "view", camera.getViewMatrix() });
	context_params.push_back({ "projection", camera.getProjectionMatrix() });
	context_params.push_back({ "model", glm::mat4(1.0f) });
	
	for (auto& entity : scene.mEntities)
	{
		if (auto* renderable = mEngine.getRenderableManager().getRenderable(entity))
		{
			glBindVertexArray(renderable->vao);
			renderable->materialInstance->bind();
			renderable->materialInstance->setUniform(context_params);
			glDrawElements(GL_TRIANGLES, renderable->indicesCount, GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);
		}
	}
}

Engine* Renderer::getEngine() const noexcept
{
	return &mEngine;
}
