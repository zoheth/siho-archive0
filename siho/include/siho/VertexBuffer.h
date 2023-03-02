#pragma once

namespace siho
{
	class VertexBuffer
	{
	public:
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;
	private:
		unsigned renderer_id_;
	};

}
