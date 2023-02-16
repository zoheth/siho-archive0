#pragma once
#include <cstdint>

#include "Entity.h"

namespace utils
{
	class EntityManager
	{
	public:
		static EntityManager& get();

		void create(size_t n, utils::Entity* entities);

	private:
		uint32_t mCurrentIndex = 1;
	};
}
