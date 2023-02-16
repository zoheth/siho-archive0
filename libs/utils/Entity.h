#pragma once

namespace utils
{
	class Entity
	{
	public:
		Entity() noexcept {}

		// Entities can be copied
		Entity(const Entity& e) noexcept = default;
		Entity(Entity&& e) noexcept = default;
		Entity& operator=(const Entity& e) noexcept = default;
		Entity& operator=(Entity&& e) noexcept = default;

		// Entities can be compared
		bool operator==(Entity e) const { return e.mIdentity == mIdentity; }
		bool operator!=(Entity e) const { return e.mIdentity != mIdentity; }

		uint32_t getId() const { return mIdentity; }
	private:
		friend class EntityManager;
		explicit Entity(uint32_t identity) noexcept : mIdentity(identity) { }

		uint32_t mIdentity = 0;
	};
}
