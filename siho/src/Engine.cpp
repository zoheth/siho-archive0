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

void siho::Engine::destroy(Engine* engine)
{
}

MaterialInstance* Engine::createMaterialInstance(const Material* material, const MaterialInstance* other,
                                                 const char* name) noexcept
{
    
}

siho::Renderer* siho::Engine::createRenderer()
{
	mRenderers.emplace_back(*this);
	return &mRenderers.back();
}

View* Engine::createView()
{
	mViews.push_back(new View())
}

Camera* siho::Engine::createCamera(Entity entity)
{
	return mCameraManager.create(entity);
}

bool siho::Engine::execute()
{

}

siho::Engine::~Engine() noexcept
{
}
