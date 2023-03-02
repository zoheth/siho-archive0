#pragma once
#include "Engine.h"
#include "View.h"
#include "RenderableManager.h"

namespace siho
{
	class Renderer
	{
	public:
		explicit Renderer(Engine& engine);
		void prepare();
		void render(View const* view);

		void draw(Renderable const* renderable);
		Engine* getEngine() const noexcept;

	private:
		Engine& mEngine;
	};
}
