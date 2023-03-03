#pragma once

class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	static void Unbind();
private:
	unsigned renderer_id_;
};

