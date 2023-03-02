#pragma once

namespace siho
{
	class IndexBuffer
	{
	public:
		IndexBuffer(const unsigned int* indices, unsigned int count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		inline unsigned int GetCount() const { return count_; }

	private:
		unsigned int renderer_id_;
		unsigned int count_;
	};
}
