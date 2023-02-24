#pragma once
#include <vector>

#include "Engine.h"
#include "utils/Entity.h"

namespace siho
{
	class Scene
	{
	public:
		explicit Scene(Engine& engine);
		~Scene() noexcept;
		void addEntity(utils::Entity entity);
	private:
		Engine& mEngine;
		std::vector<utils::Entity> mEntities;
	};
}
