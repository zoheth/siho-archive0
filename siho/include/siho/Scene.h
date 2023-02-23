#pragma once
#include <vector>
#include "utils/Entity.h"

namespace siho
{
	class Scene
	{
	public:
		std::vector<utils::Entity> mEntities;
		void addEntity(utils::Entity entity);
	private:
	};
}
