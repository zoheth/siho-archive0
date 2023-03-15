#pragma once
#include <vector>

class RenderTarget {
public:
    RenderTarget(int width, int height);
    RenderTarget(int width, int height, int color_attachments_count);
    RenderTarget(int width, int height, int color_attachments_count, int render_buffer_count);

    void bind() const;
    static void unbind();

    [[nodiscard]] int width() const { return width_; }
    [[nodiscard]] int height() const { return height_; }
    [[nodiscard]] unsigned int color_buffer() const { return color_attachments_[0]; }
    [[nodiscard]] unsigned int color_buffer(const size_t i) const { return color_attachments_[i]; }
    [[nodiscard]] unsigned int depth_stencil_buffer() const { return depth_stencil_buffers_[0]; }

private:
    void CreateFrameBuffer(unsigned int color_attachments_count=1, unsigned int render_buffer_count=1);

    int width_;
    int height_;

    unsigned int fbo_{};
    std::vector<unsigned int> color_attachments_{};
    std::vector<unsigned int> depth_stencil_buffers_{};
};
