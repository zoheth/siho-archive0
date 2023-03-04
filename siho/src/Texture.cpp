#include <iostream>
#include <siho/Texture.h>

#include <stb_image.h>
#include <glad/glad.h>

Texture::Texture(const std::string& path, const bool gamma)
	:file_path_(path)
{
    stbi_set_flip_vertically_on_load(1);

    if(unsigned char* data = stbi_load(path.c_str(), &width_, &height_, &bpp_, 0))
	{
        GLenum internal_format;
        GLenum data_format;
        if (bpp_ == 1)
            internal_format = data_format = GL_RED;
        else if (bpp_ == 3)
        {
            internal_format = gamma ? GL_SRGB : GL_RGB;
            data_format = GL_RGB;
        }
        else if (bpp_ == 4)
        {
            internal_format = gamma ? GL_SRGB_ALPHA : GL_RGBA;
            data_format = GL_RGBA;
        }

        glGenTextures(1, &id_);
        glBindTexture(GL_TEXTURE_2D, id_);
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width_, height_, 0, data_format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
	}
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
}

Texture::~Texture()
{
    //glDeleteTextures(1, &id_);
}

void Texture::Bind(unsigned slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
};
