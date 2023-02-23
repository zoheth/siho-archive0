#include <siho/Renderer.h>
#include <siho/Scene.h>

using namespace siho;


Renderer::Renderer(Engine& engine):
	mEngine(engine)
{
}

void Renderer::prepare()
{
	
}

void siho::Renderer::render(View const* view)
{
	const Scene& scene = *view->mScene;
	Camera& camera = *view->mCullingCamera;
}
