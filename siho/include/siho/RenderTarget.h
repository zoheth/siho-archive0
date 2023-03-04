#pragma once

class RenderTarget {
public:
    RenderTarget(int width, int height);

    void bind() const;
    static void unbind();

    [[nodiscard]] int width() const { return width_; }
    [[nodiscard]] int height() const { return height_; }
    [[nodiscard]] int texture_id() const { return color_attachment_; }
    [[nodiscard]] unsigned int depth_stencil_buffer() const { return depth_stencil_buffer_; }

private:
    void CreateFrameBuffer();

    int width_;
    int height_;

    unsigned int fbo_{};
    unsigned int color_attachment_{};
    unsigned int depth_stencil_buffer_{};
};
