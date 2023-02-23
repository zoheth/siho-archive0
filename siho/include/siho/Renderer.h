#pragma once
#include "Engine.h"
#include "View.h"

namespace siho
{
	class Renderer
	{
	public:
		explicit Renderer(Engine& engine);
		void prepare();
		void render(View const* view);
		Engine* getEngine() noexcept;

	private:
		Engine& mEngine;
	};
}
