#include <siho/MaterialInstance.h>
#include <siho/Material.h>

#include <utility>

using namespace siho;

template void MaterialInstance::setParameter<float>(const std::string name, const float& value) noexcept;
template void MaterialInstance::setParameter<uint32_t>(const std::string name, const uint32_t& value) noexcept;
template void MaterialInstance::setParameter<glm::vec2>(const std::string name, const glm::vec2& value) noexcept;
template void MaterialInstance::setParameter<glm::vec3>(const std::string name, const glm::vec3& value) noexcept;
template void MaterialInstance::setParameter<glm::vec4>(const std::string name, const glm::vec4& value) noexcept;
template void MaterialInstance::setParameter<glm::mat2>(const std::string name, const glm::mat2& value) noexcept;
template void MaterialInstance::setParameter<glm::mat3>(const std::string name, const glm::mat3& value) noexcept;
template void MaterialInstance::setParameter<glm::mat4>(const std::string name, const glm::mat4& value) noexcept;


template <typename T>
void MaterialInstance::setParameter(const std::string name, const T& value) noexcept
{
	mParameters.push_back({name, value});
}

MaterialInstance::MaterialInstance(Engine& engine, const Material* material, std::string name)
	:mMaterial(material), mName(std::move(name))
{

}

MaterialInstance* MaterialInstance::duplicate(MaterialInstance const* other, const char* name) noexcept
{
	Material const* const material = other->getMaterial();
	Engine& engine = material->getEngine();
	return engine.createMaterialInstance(material, other, name);
}

void MaterialInstance::bind()
{
	mMaterial->mShader->use();
	setUniform(mParameters);
}

void MaterialInstance::setUniform(const std::vector<Parameter>& params) const noexcept
{
	for (auto [name, value] : params)
	{
		if (std::holds_alternative<float>(value))
		{
			mMaterial->mShader->setFloat(name, std::get<float>(value));
		}
		else if (std::holds_alternative<uint32_t>(value))
		{
			mMaterial->mShader->setInt(name, std::get<uint32_t>(value));
		}
		else if (std::holds_alternative<glm::vec2>(value))
		{
			mMaterial->mShader->setVec2(name, std::get<glm::vec2>(value));
		}
		else if (std::holds_alternative<glm::vec3>(value))
		{
			mMaterial->mShader->setVec3(name, std::get<glm::vec3>(value));
		}
		else if (std::holds_alternative<glm::vec4>(value))
		{
			mMaterial->mShader->setVec4(name, std::get<glm::vec4>(value));
		}
		else if (std::holds_alternative<glm::mat2>(value))
		{
			mMaterial->mShader->setMat2(name, std::get<glm::mat2>(value));
		}
		else if (std::holds_alternative<glm::mat3>(value))
		{
			mMaterial->mShader->setMat3(name, std::get<glm::mat3>(value));
		}
		else if (std::holds_alternative<glm::mat4>(value))
		{
			mMaterial->mShader->setMat4(name, std::get<glm::mat4>(value));
		}
	}
}
