#include <siho/CameraManager.h>
#include <siho/Engine.h>

using namespace siho;

CameraManager::CameraManager(Engine& engine)
	: mEngine(engine)
{

}

Camera* CameraManager::create(utils::Entity entity)
{
	Engine& engine= mEngine;
	auto camera = new Camera(mEngine, entity);
	return camera;
}
