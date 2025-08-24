#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>
#include <Log/Log.hpp>

#include <Graphics/Texture/Texture.hpp>

smpl::Texture::Texture()
    : texture_id { 0 }
    , width      { 0 }
    , height     { 0 }
    , channels   { 0 }
{
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // TODO settings should be in other place
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

smpl::Texture::~Texture()
{
}

bool smpl::Texture::loadFromFile(const std::string& file_path)
{
    unsigned char* data = stbi_load(file_path.c_str(), &width, &height, &channels, 0);
    if (data)
    {
        stbi__vertical_flip(data, width, height, channels);


        // TODO Incapsulate it + I`m not check out green & blue chanel. Or it isn`t matter?
        GLenum internal_format;
        GLenum format;

        if (channels == 1) 
        {
            internal_format = GL_RED;
            format = GL_RED;
        }
        else if (channels == 2) 
        {
            internal_format = GL_RG;
            format = GL_RG;
        }
        else if (channels == 3) 
        {
            internal_format = GL_RGB;
            format = GL_RGB;
        }
        else if (channels == 4) 
        {
            internal_format = GL_RGBA;
            format = GL_RGBA;
        }
        else 
        {
            LOG_ERROR("Unsupported number of channels: %i", channels);
            stbi_image_free(data);
            return false;
        }

        // TODO. Attach texture should be in other place?
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        LOG_ERROR("Failed to load texture");
        stbi_image_free(data);
        data = nullptr;
        return false;
    }

    stbi_image_free(data);
    data = nullptr;
    return true;
}