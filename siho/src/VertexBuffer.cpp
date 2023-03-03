#include <glad/glad.h>
#include <siho/VertexBuffer.h>


VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    glGenBuffers(1, &renderer_id_);
    glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &renderer_id_);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
}

void VertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}