#pragma once
#include <string>

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

        // frees driver resources, object becomes invalid
        void bind(Engine& engine);
        void unbind(Engine& engine) noexcept;

        Material const* getMaterial() const noexcept { return mMaterial; }


        template<typename T>
        void setParameter(const char* name, const T& value) noexcept;



        const char* getName() const noexcept { return mName.c_str(); }


    private:
        friend class Material;

        Material const* mMaterial;
        std::string mName;
    };

    inline MaterialInstance* MaterialInstance::duplicate(MaterialInstance const* other, const char* name) noexcept
    {
        Material const* const material = other->getMaterial();
        Engine& engine = material->getEngine();
        return engine.createMaterialInstance(material, other, name);
    }
}
