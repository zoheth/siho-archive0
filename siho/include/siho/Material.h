#pragma once

#include "Engine.h"
#include "MaterialInstance.h"
#include "Shader.h"

// 枚举类型，表示材质的反射模式
enum class ReflectionMode {
    NONE, // 无反射
    CUBE_MAP, // 立方体贴图反射
    PLANAR // 平面反射
};

enum class CullingMode : uint8_t {
    NONE,               //!< No culling, front and back faces are visible
    FRONT,              //!< Front face culling, only back faces are visible
    BACK,               //!< Back face culling, only front faces are visible
    FRONT_AND_BACK      //!< Front and Back, geometry is not visible
};

namespace siho {
    class MaterialInstance;

    class Material {
    public:
	    explicit Material(Engine& engine);
        ~Material();

        MaterialInstance* createInstance(const char* name) const noexcept;

        MaterialInstance const* getInstance() const;

        MaterialInstance* getInstance();

        uint64_t getInstanceId() const;

        uint8_t getVariant() const;

        void prepareProgram() const noexcept;

        uint64_t getMaterialProperties() const;

        ReflectionMode getReflectionMode() const;

        Engine& getEngine() const noexcept { return mEngine; }

        bool isDefaultMaterial() const;

        void setVariant(uint8_t variant) noexcept;

        void setMaterialProperties(uint64_t properties) noexcept;

        void setReflectionMode(ReflectionMode mode) noexcept;

        void setDoubleSided(bool doubleSided) noexcept;

        void setIsDefaultMaterial(bool isDefaultMaterial) noexcept;
    private:


        Shader* mShader;

        Engine& mEngine;

        MaterialInstance mDefaultInstance;
    };
}
