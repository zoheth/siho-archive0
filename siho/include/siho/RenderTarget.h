#pragma once

#include <glad/glad.h>

class RenderTarget {
public:
    RenderTarget(int width, int height);

    void bind() const;
    static void unbind();

    [[nodiscard]] int width() const { return width_; }
    [[nodiscard]] int height() const { return height_; }
    [[nodiscard]] GLuint texture_id() const { return color_attachment_; }
    [[nodiscard]] GLuint depth_stencil_buffer() const { return depth_stencil_buffer_; }

private:
    void CreateFrameBuffer();

    int width_;
    int height_;

    GLuint fbo_{};
    GLuint color_attachment_{};
    GLuint depth_stencil_buffer_{};
};
