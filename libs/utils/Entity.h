#pragma once

namespace utils
{
	class Entity
	{
	public:
		Entity() noexcept {}

		uint32_t getId() const { return id_; }
	private:
		uint32_t id_ = 0;
	};
}
