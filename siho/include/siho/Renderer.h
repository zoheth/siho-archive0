#pragma once

namespace siho
{
	class View;

	class Renderer
	{
	public:
		void clear() const;
		void render(const View& view) const;
	};
}
