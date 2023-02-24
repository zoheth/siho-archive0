#include <siho/Scene.h>

#include "siho/Engine.h"

using namespace siho;

Scene::Scene(Engine& engine) :
	mEngine(engine)
{
}

void siho::Scene::addEntity(utils::Entity entity)
{
	mEntities.push_back(entity);
}
