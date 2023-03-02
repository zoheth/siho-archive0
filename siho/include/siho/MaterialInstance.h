#pragma once

#include <string>
#include <vector>
#include <variant>
#include <glm/glm.hpp>

namespace siho
{
    class Engine;
    class Material;
	class MaterialInstance {
    public:

        MaterialInstance(Engine& engine, const Material * material, std::string name);

        static MaterialInstance* duplicate(MaterialInstance const* other, const char* name) noexcept;

        void bind();
        void unbind() noexcept;

        Material const* getMaterial() const noexcept { return mMaterial; }

        struct Parameter
        {
	        std::string name{};
            std::variant<float, uint32_t,
            			glm::vec2, glm::vec3, glm::vec4,
        				glm::mat2, glm::mat3, glm::mat4> value;
        };
        template<typename T>
        void setParameter(const std::string name, const T& value) noexcept;
        void setUniform(const std::vector<Parameter>& params) const noexcept;

		const char* getName() const noexcept { return mName.c_str(); }


    private:
        friend class Material;
        
        const Material * mMaterial{};
        std::string mName{};

        std::vector<Parameter> mParameters{};
        std::vector<Parameter> mContextParameters{};
    };
}
