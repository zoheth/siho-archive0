#include <siho/Engine.h>
#include <siho/Renderer.h>
#include <utils/Entity.h>


using namespace siho;
using namespace utils;

Engine* Engine::create()
{
	auto* instance = new Engine();
	return instance;
}

void Engine::destroy(Engine* engine)
{
}

MaterialInstance* Engine::createMaterialInstance(const Material* material, const MaterialInstance* other, const char* name) noexcept
{
    
}

Renderer* Engine::createRenderer()
{
	mRenderers.emplace_back(*this);
	return &mRenderers.back();
}

Scene* Engine::createScene()
{
	mScenes.emplace_back(*this);
	return &mScenes.back();
}

View* Engine::createView()
{
	mViews.push_back(*new View(*this));
}

Camera* Engine::createCamera(const Entity entity)
{
	return mCameraManager.create(entity);
}

void Engine::createRenderable(const RenderableManager::Builder& builder, utils::Entity entity)
{
	mRenderableManager.
}

bool Engine::execute()
{

}
