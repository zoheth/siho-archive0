#pragma once
#include <cstdint>

#include "Scene.h"

namespace siho
{
	class View
	{
	public:
        void setScene(Scene* scene);
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

