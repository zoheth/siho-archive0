#pragma once
#include "MaterialInstance.h"
#include "../src/Shader.h"

namespace siho {
    class MaterialInstance;

    class Material {
    public:
	    explicit Material(Engine& engine);
        ~Material();

        MaterialInstance* createInstance(const char* name) const noexcept;

        Engine& getEngine() const noexcept { return mEngine; }

    private:
        friend class MaterialInstance;
        Shader* mShader{};
        Engine& mEngine;

        MaterialInstance mDefaultInstance;
    };
}
