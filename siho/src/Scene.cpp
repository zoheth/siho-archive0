#include <siho/Scene.h>

#include "siho/Engine.h"

using namespace siho;

Scene::Scene(Engine& engine) :
	mEngine(engine)
{
}

void Scene::prepare()
{
	for(auto& entity : mEntities)
	{
		if(auto* renderable = mEngine.getRenderableManager().getRenderable(entity))
		{
			
		}
	}
}

void siho::Scene::addEntity(utils::Entity entity)
{
	mEntities.push_back(entity);
}
