#pragma once
#include <glm/glm.hpp>

#include <string>

class Shader
{
public:
    Shader(const char* vertex_path, const char* fragment_path, const char* geometry_path = nullptr);

    void use() const;
    [[nodiscard]] unsigned int program_id() const { return program_id_; }

    void setBool(const std::string& name, bool value) const;
    void setUInt(const std::string& name, unsigned int value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec2(const std::string& name, float x, float y) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setVec4(const std::string& name, float x, float y, float z, float w);
    void setMat2(const std::string& name, const glm::mat2& mat) const;
    void setMat3(const std::string& name, const glm::mat3& mat) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
    unsigned int program_id_;
    static void checkCompileErrors(unsigned int shader, const std::string& type);
};
