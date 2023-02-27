#include <siho/MaterialInstance.h>

using namespace siho;

template <typename T>
void MaterialInstance::setParameter(const char* name, const T& value) noexcept
{
	mParameters.push_back({name, value});
}

void MaterialInstance::bind()
{
	mMaterial->mShader->use();
	for (auto [name, value] : mParameters)
	{
		if (std::holds_alternative<float>(value))
		{
			mMaterial->mShader->setFloat(name, std::get<float>(value));
		}
		else if (std::holds_alternative<glm::vec3>(value))
		{
			mMaterial->mShader->setVec3(name, std::get<glm::vec3>(value));
		}
		else if (std::holds_alternative<glm::vec4>(value))
		{
			mMaterial->mShader->setVec4(name, std::get<glm::vec4>(value));
		}
		else if (std::holds_alternative<uint32_t>(value))
		{
			mMaterial->mShader->setInt(name, std::get<uint32_t>(value));
		}
	}
}
