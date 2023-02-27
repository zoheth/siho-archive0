#pragma once
#include <string>
#include <variant>
#include <glm/glm.hpp>

#include "Engine.h"
#include "Material.h"

namespace siho
{
	class MaterialInstance {
    public:

        MaterialInstance(Engine& engine, MaterialInstance const* other, const char* name);
        MaterialInstance(Engine& engine, Material const* material, const char* name);
        ~MaterialInstance() noexcept;

        static MaterialInstance* duplicate(MaterialInstance const* other, const char* name) noexcept;

        void bind();
        void unbind() noexcept;

        Material const* getMaterial() const noexcept { return mMaterial; }

        struct Parameter
        {
            char* name{};
            std::variant<glm::vec3, glm::vec4, float, uint32_t> value;
        };
        template<typename T>
        void setParameter(const char* name, const T& value) noexcept;


		const char* getName() const noexcept { return mName.c_str(); }


    private:
        friend class Material;
        
        Material const* mMaterial;
        std::string mName;

        std::vector<Parameter> mParameters;
    };


	inline MaterialInstance* MaterialInstance::duplicate(MaterialInstance const* other, const char* name) noexcept
    {
        Material const* const material = other->getMaterial();
        Engine& engine = material->getEngine();
        return engine.createMaterialInstance(material, other, name);
    }
}
