#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

class UniformObject {
public:
    UniformObject() {}

    void set(const std::string& name, bool value) const;
    void set(const std::string& name, int value) const;
    void set(const std::string& name, float value) const;
    void set(const std::string& name, const glm::vec2& value) const;
    void set(const std::string& name, const glm::vec3& value) const;
    void set(const std::string& name, const glm::vec4& value) const;
    void set(const std::string& name, const glm::mat2& mat) const;
    void set(const std::string& name, const glm::mat3& mat) const;
    void set(const std::string& name, const glm::mat4& mat) const;

private:
    std::unordered_map<std::string, int> location_cache_;

    int getLocation(const std::string& name) const;
};
