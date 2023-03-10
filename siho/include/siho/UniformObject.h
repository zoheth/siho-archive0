#pragma once
#include <string>
#include <unordered_map>
#include <variant>
#include <glm/glm.hpp>

using std::string;

namespace uniforms
{
	static const string kModel = "model";
	static const string kView = "view";
	static const string kProjection = "projection";

	static const string kCameraPosition = "u_CamPos";

	static const string kBaseColorFactor = "u_Material.baseColorFactor";
	static const string kSpecularColorFactor = "u_Material.specularColorFactor";
	static const string kEmissiveFactor = "u_Material.emissiveFactor";

	static const string kMetallicFactor = "u_Material.metallicFactor";
	static const string kRoughnessFactor = "u_Material.roughnessFactor";
	static const string kIor = "u_Material.ior";

	static const string kBaseColorTexture = "u_Material.baseColorTexture";
	static const string kMetallicRoughnessTexture = "u_Material.metallicRoughnessTexture";
	static const string kNormalTexture = "u_Material.normalTexture";
	static const string kEmissiveTexture = "u_Material.emissiveTexture";

	static const string kPointLightPrefix = "u_PointLights";
	static const string kSpotLightPrefix = "u_SpotLights";
	static const string kPointLightsCount = "u_PointLightsCount";
	static const string kSpotLightsCount = "u_SpotLightsCount";

	enum class Light
	{
		POSITION,
		DIRECTION,
		COLOR,
		INTENSITY,
		CUTOFF,
		OUTER_CUTOFF
	};
	inline std::string PointLight(const int index, const Light uniform)
	{
		return kPointLightPrefix + "[" + std::to_string(index) + "]." + [&]
		{
			switch (uniform)
			{
			case Light::POSITION: return "position";
			case Light::COLOR: return "color";
			case Light::INTENSITY: return "intensity";
			default: return "";
			}
		}();
	}

	inline std::string SpotLight(const int index, const Light uniform)
	{
		return kSpotLightPrefix + "[" + std::to_string(index) + "]." + [&]
		{
			switch (uniform)
			{
			case Light::POSITION: return "position";
			case Light::DIRECTION: return "direction";
			case Light::COLOR: return "color";
			case Light::INTENSITY: return "intensity";
			case Light::CUTOFF: return "cutoff";
			case Light::OUTER_CUTOFF: return "outerCutoff";
			default: return "";
			}
		}();
	}

	static const string kScreenTexture = "u_ScreenTexture";
	static const string kScreenTextureCount = "u_ScreenTextureCount";
}

class Shader;

class UniformObject
{
public:
	using UniformData =
	std::variant<bool, float, int, unsigned int,
	             glm::vec2, glm::vec3, glm::vec4,
	             glm::mat2, glm::mat3, glm::mat4>;

	template <typename T>
	void set(const std::string& name, const T& value);

	void apply(const Shader& shader) const;

private:
	std::unordered_map<std::string, UniformData> uniform_data_{};
};
