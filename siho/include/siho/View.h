#pragma once
#include "../src/PerViewUniforms.h"

#include <cstdint>
#include <string>


namespace siho
{
    class Camera;
    class Scene;

	class View
	{
	public:
        explicit View(Engine& engine);

        void setScene(siho::Scene* scene) { mScene = scene; }

        void setName(const std::string& name) noexcept {
            mName = name;
        }
        void setCullingCamera(Camera* camera) noexcept { mCullingCamera = camera; }

        Camera* getCullingCamera() const noexcept { return mCullingCamera; }
        Scene* getScene() const noexcept { return mScene; }


    private:
        std::string mName{};
        //mutable PerViewUniforms mPerViewUniforms;

        Scene *mScene = nullptr;
        Camera* mCullingCamera = nullptr;

	};

    struct Viewport {
        int32_t left;       //!< left coordinate in window space.
        int32_t bottom;     //!< bottom coordinate in window space.
        uint32_t width;     //!< width in pixels
        uint32_t height;    //!< height in pixels
        //! get the right coordinate in window space of the viewport
        int32_t right() const noexcept { return left + static_cast<int32_t>(width); }
        //! get the top coordinate in window space of the viewport
        int32_t top() const noexcept { return bottom + static_cast<int32_t>(height); }
    };
}

