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
        void terminate(Engine& engine);

        void commit(Engine& engine) noexcept;


        Material const* getMaterial() const noexcept { return mMaterial; }


        template<typename T>
        void setParameter(const char* name, const T& value) noexcept;


        void setScissor(int32_t left, int32_t bottom, uint32_t width, uint32_t height) noexcept;

        void unsetScissor() noexcept;

        void setPolygonOffset(float scale, float constant) noexcept;

        void setMaskThreshold(float threshold) noexcept;

        void setSpecularAntiAliasingVariance(float variance) noexcept;

        void setSpecularAntiAliasingThreshold(float threshold) noexcept;

        void setDoubleSided(bool doubleSided) noexcept;


        void setColorWrite(bool enable) noexcept;

        void setDepthWrite(bool enable) noexcept;

        void setDepthCulling(bool enable) noexcept;


        void setShadowMultiplier(bool shadowMultiplier) noexcept;

        void setSpecularAntiAliasing(bool specularAntiAliasing) noexcept;


        float getMaskThreshold() const noexcept { return mMaskThreshold; }

        float getSpecularAntiAliasingVariance() const noexcept { return mSpecularAntiAliasingVariance; }

        float getSpecularAntiAliasingThreshold() const noexcept { return mSpecularAntiAliasingThreshold; }

        const char* getName() const noexcept { return mName.c_str(); }


    private:
        friend class Material;

        Material const* mMaterial;
        std::string mName;
        float mMaskThreshold = 0.4f;
        float mSpecularAntiAliasingVariance = 0.0f;
        float mSpecularAntiAliasingThreshold = 0.0f;
    };

    inline MaterialInstance* MaterialInstance::duplicate(MaterialInstance const* other, const char* name) noexcept
    {
        Material const* const material = other->getMaterial();
        Engine& engine = material->getEngine();
        return engine.createMaterialInstance(material, other, name);
    }
}
