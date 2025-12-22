#include <stdafx.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>
#include <Log/Log.hpp>

#include <Graphics/Texture/Texture.hpp>

smpl::Texture::Texture()
    : m_texture_id { 0 }
    , m_width      { 0 }
    , m_height     { 0 }
    , m_channels   { 0 }
{
}

smpl::Texture::~Texture()
{
    if (m_texture_id != 0)
        glDeleteTextures(1, &m_texture_id);
}

bool smpl::Texture::loadFromFile(const std::string& file_path)
{
    if (m_texture_id != 0)
        glDeleteTextures(1, &m_texture_id);

    glGenTextures(1, &m_texture_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* data = stbi_load(file_path.c_str(), &m_width, &m_height, &m_channels, 0);
    if (data)
    {
        stbi__vertical_flip(data, m_width, m_height, m_channels);

        GLenum internal_format;
        GLenum format;

        if (m_channels == 1)
        {
            internal_format = GL_RED;
            format = GL_RED;
        }
        else if (m_channels == 2)
        {
            internal_format = GL_RG;
            format = GL_RG;
        }
        else if (m_channels == 3)
        {
            internal_format = GL_RGB;
            format = GL_RGB;
        }
        else if (m_channels == 4)
        {
            internal_format = GL_RGBA;
            format = GL_RGBA;
        }
        else
        {
            LOG_ERROR("Unsupported number of channels: %i", m_channels);
            stbi_image_free(data);
            return false;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        LOG_DEBUG("Texture loaded: \"{}\".", file_path);
    }
    else
    {
        LOG_ERROR("Failed to load texture");
        stbi_image_free(data);
        return false;
    }

    stbi_image_free(data);
    return true;
}

unsigned int smpl::Texture::getTextureID() const
{
    return m_texture_id;
}

int smpl::Texture::getHeight() const
{
    return m_height;
}

int smpl::Texture::getWidth() const
{
    return m_width;
}

int smpl::Texture::getChannels() const
{
    return m_channels;
}