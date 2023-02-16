#pragma once
#include "Engine.h"
#include "View.h"

namespace siho
{
	class Renderer
	{
	public:
		void render(View const* view);
		Engine* getEngine() noexcept;
	};
}
