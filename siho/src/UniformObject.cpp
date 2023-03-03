#include <stdexcept>
#include <siho/UniformObject.h>
#include <siho/Shader.h>


template void UniformObject::set<bool>(const std::string& name, const bool& value);
template void UniformObject::set<float>(const std::string& name, const float& value);
template void UniformObject::set<int>(const std::string& name, const int& value);
template void UniformObject::set<glm::vec2>(const std::string& name, const glm::vec2& value);
template void UniformObject::set<glm::vec3>(const std::string& name, const glm::vec3& value);
template void UniformObject::set<glm::vec4>(const std::string& name, const glm::vec4& value);
template void UniformObject::set<glm::mat2>(const std::string& name, const glm::mat2& value);
template void UniformObject::set<glm::mat3>(const std::string& name, const glm::mat3& value);
template void UniformObject::set<glm::mat4>(const std::string& name, const glm::mat4& value);

template <typename T>
void UniformObject::set(const std::string& name, const T& value)
{
	uniform_data_[name] = value;
}

void UniformObject::apply(const Shader& shader) const
{
	for (auto [name, value] : uniform_data_)
	{
		if (std::holds_alternative<bool>(value))
		{
			shader.setBool(name, std::get<bool>(value));
		}
		else if (std::holds_alternative<float>(value))
		{
			shader.setFloat(name, std::get<float>(value));
		}
		else if (std::holds_alternative<int>(value))
		{
			shader.setInt(name, std::get<int>(value));
		}
		else if (std::holds_alternative<glm::vec2>(value))
		{
			shader.setVec2(name, std::get<glm::vec2>(value));
		}
		else if (std::holds_alternative<glm::vec3>(value))
		{
			shader.setVec3(name, std::get<glm::vec3>(value));
		}
		else if (std::holds_alternative<glm::vec4>(value))
		{
			shader.setVec4(name, std::get<glm::vec4>(value));
		}
		else if (std::holds_alternative<glm::mat2>(value))
		{
			shader.setMat2(name, std::get<glm::mat2>(value));
		}
		else if (std::holds_alternative<glm::mat3>(value))
		{
			shader.setMat3(name, std::get<glm::mat3>(value));
		}
		else if (std::holds_alternative<glm::mat4>(value))
		{
			shader.setMat4(name, std::get<glm::mat4>(value));
		}
		else
		{
			throw std::runtime_error("UniformObject::apply: unknown uniform type");
		}
	}
}
