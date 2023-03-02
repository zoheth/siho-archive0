#pragma once

#include <vector>

namespace utils
{
	class Entity;
}

namespace siho
{
	class Engine;
	class Scene
	{
	public:
		explicit Scene(Engine& engine);

		void prepare();

		void addEntity(utils::Entity entity);

	private:
		friend class Renderer;
		Engine& mEngine;
		std::vector<utils::Entity> mEntities{};
	};
}
