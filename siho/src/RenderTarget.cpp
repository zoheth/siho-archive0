#include <siho/RenderTarget.h>

#include <stdexcept>
#include <glad/glad.h>


RenderTarget::RenderTarget(const int width, const int height)
    : width_(width), height_(height)
{
    CreateFrameBuffer();
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

void RenderTarget::CreateFrameBuffer()
{
    glGenFramebuffers(1, &fbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

    glGenTextures(1, &color_attachment_);
    glBindTexture(GL_TEXTURE_2D, color_attachment_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attachment_, 0);

    glGenRenderbuffers(1, &depth_stencil_buffer_);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_stencil_buffer_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_stencil_buffer_);

    if (const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER); status != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Failed to create framebuffer");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
