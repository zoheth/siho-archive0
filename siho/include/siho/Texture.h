#pragma once
#include <string>

class Texture
{

public:
	explicit Texture(const std::string& path, const bool gamma=false);
    ~Texture();

	[[nodiscard]] unsigned int id() const { return id_; }

    void Bind(unsigned int slot = 0) const;
    static void Unbind();

	[[nodiscard]] int width() const { return width_; }
	[[nodiscard]] int height() const { return height_; }

private:
    unsigned int id_{};
    std::string file_path_;
    int width_{}, height_{}, bpp_{};
};
