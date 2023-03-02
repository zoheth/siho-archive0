#pragma once



#include <glm/glm.hpp>
#include <cstdint>
#include <unordered_map>

namespace utils
{
	class Entity;
}

namespace siho
{
	class Engine;
	struct LightData
	{
		
	};
	class LightManager
	{
		struct BuilderDetails;
	public:
		explicit LightManager(Engine& engine) :mEngine(engine) {}
		bool hasComponent(utils::Entity e) const noexcept;

		//! Denotes the type of the light being created.
		enum class type : uint8_t {
			SUN,            //!< Directional light that also draws a sun's disk in the sky.
			DIRECTIONAL,    //!< Directional light, emits light in a given direction.
			POINT,          //!< Point light, emits light from a position, in all directions.
			FOCUSED_SPOT,   //!< Physically correct spot light.
			SPOT,           //!< Spot light with coupling of outer cone and illumination disabled.
		};

		//! Use Builder to construct a Light object instance
		class Builder
		{
		public:
			Builder& position(const glm::vec3& position) noexcept;
			Builder& direction(const glm::vec3& direction) noexcept;
			Builder& color(const glm::vec3& color) noexcept;
			Builder& intensity(float intensity) noexcept;
			Builder& intensity(float watts, float efficiency) noexcept;
			Builder& falloff(float radius) noexcept;
			enum Result { ERROR = -1, SUCCESS = 0 };
			Result build(Engine& engine);
		};

		void create(const LightManager::Builder& builder);

	private:
		Engine& mEngine;
		uint32_t mLightCount = 0;
		std::vector<LightData> mLightArray;
		
	};
}
