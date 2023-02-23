#include "EntityManager.h"

using namespace utils;

EntityManager& EntityManager::get()
{
	static EntityManager instance;
	return instance;
}

void EntityManager::create(size_t n, Entity* entities)
{
	uint32_t current_index = mCurrentIndex;
	for(size_t i = 0; i < n; ++i)
	{
		entities[i] = Entity{ current_index };
		++current_index;
	}
	mCurrentIndex += n;
}
