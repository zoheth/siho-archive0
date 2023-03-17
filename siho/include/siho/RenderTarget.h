#pragma once
#include <iostream>
#include <vector>

class RenderTarget {
public:
    RenderTarget(int width, int height);
    RenderTarget(int width, int height, int color_attachments_count, int render_buffer_count);

    void bind() const;
    static void unbind();

    [[nodiscard]] int width() const { return width_; }
    [[nodiscard]] int height() const { return height_; }
    [[nodiscard]] unsigned int color_buffer() const
    {
        if (color_attachments_.empty())
            std::cerr << "No color attachments" << std::endl;
        return color_attachments_[0];
    }
    [[nodiscard]] unsigned int color_buffer(const size_t i) const
    {
        if (color_attachments_.size()<=i)
            std::cerr << "No color attachments index" << std::endl;
	    return color_attachments_[i];
    }
    [[nodiscard]] unsigned int depth_stencil_buffer() const
    {
        if (depth_stencil_buffers_.empty())
            std::cerr << "No depth attachments" << std::endl;
	    return depth_stencil_buffers_[0];
    }

    static RenderTarget* CreateShadowTarget(int width, int height);

private:
    void CreateFrameBuffer(unsigned int color_attachments_count, unsigned int render_buffer_count);

    int width_;
    int height_;

    unsigned int fbo_{};
    std::vector<unsigned int> color_attachments_{};
    std::vector<unsigned int> depth_stencil_buffers_{};
};
