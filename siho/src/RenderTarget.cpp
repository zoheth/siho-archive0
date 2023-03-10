#include <siho/RenderTarget.h>

#include <stdexcept>
#include <glad/glad.h>


RenderTarget::RenderTarget(const int width, const int height)
    : width_(width), height_(height)
{
    CreateFrameBuffer();
}

RenderTarget::RenderTarget(int width, int height, int color_attachments_count)
{
	width_ = width;
	height_ = height;
	CreateFrameBuffer(color_attachments_count);
}


void RenderTarget::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glViewport(0, 0, width_, height_);
}

void RenderTarget::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTarget::CreateFrameBuffer(unsigned int color_attachments_count, unsigned int render_buffer_count)
{
    glGenFramebuffers(1, &fbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

    std::vector<GLenum> attachments(color_attachments_count);
    for (unsigned int i = 0; i < color_attachments_count; i++)
    {
        unsigned int color_attachment;
        glGenTextures(1, &color_attachment);
        glBindTexture(GL_TEXTURE_2D, color_attachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, color_attachment, 0);
        attachments[i] = GL_COLOR_ATTACHMENT0 + i;
        color_attachments_.push_back(color_attachment);
    }
    for (unsigned int i = 0; i < render_buffer_count; i++)
    {
	    unsigned int render_buffer;
		glGenRenderbuffers(1, &render_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, render_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, render_buffer);
		depth_stencil_buffers_.push_back(render_buffer);
    }
    glDrawBuffers(static_cast<int>(color_attachments_count), attachments.data());

    if (const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER); status != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Failed to create framebuffer");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
