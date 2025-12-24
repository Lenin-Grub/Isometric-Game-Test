#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <Log/Log.hpp>
#include <Graphics/Texture/Texture.hpp>

namespace smpl
{
    Texture::Texture()
        : m_texture_id{ 0 }
        , m_width{ 0 }
        , m_height{ 0 }
        , m_channels{ 0 }
    {
    }

    Texture::~Texture()
    {
        if (m_texture_id != 0)
            glDeleteTextures(1, &m_texture_id);
    }

    bool Texture::loadFromFile(const std::string& file_path)
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

    unsigned int Texture::getTextureID() const
    {
        return m_texture_id;
    }

    int Texture::getHeight() const
    {
        return m_height;
    }

    int Texture::getWidth() const
    {
        return m_width;
    }

    int Texture::getChannels() const
    {
        return m_channels;
    }



    Texture2D::Texture2D()
        : m_width(0)
        , m_height(0)
        , m_channels(4)
        , m_internal_format(GL_RGBA)
        , m_image_format(GL_RGBA)
        , m_wrap_s(GL_REPEAT)
        , m_wrap_t(GL_REPEAT)
        , m_filter_min(GL_LINEAR)
        , m_filter_max(GL_LINEAR)
        , m_id(0)
    {
    }

    Texture2D::~Texture2D()
    {
        if (m_id != 0)
        {
            glDeleteTextures(1, &m_id);
            m_id = 0;
        }
    }

    void Texture2D::generate(int width, int height, unsigned char* data)
    {
        m_width  = width;
        m_height = height;

        // create Texture
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexImage2D(GL_TEXTURE_2D, 0, m_internal_format, width, height, 0, m_image_format, GL_UNSIGNED_BYTE, data);
        
        // set Texture wrap and filter modes
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrap_s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrap_t);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filter_min);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filter_max);
        
        // unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture2D::bind() const
    {
        if (m_id != 0)
            glBindTexture(GL_TEXTURE_2D, m_id);
        else
            glBindTexture(GL_TEXTURE_2D, 0);
    }

    bool Texture2D::loadTextureFromFile(const std::filesystem::path& path, bool alpha)
    {
        glGenTextures(1, &m_id);

        if (!std::filesystem::exists(path))
        {
            LOG_ERROR("Texture file does not exist. Path: {}", path.string());
            return false;
        }

        if (!std::filesystem::is_regular_file(path))
        {
            LOG_ERROR("Path is not a regular file (might be a directory or other). Path: {}", path.string());
            return false;
        }

        unsigned char* data = nullptr;

        std::string path_str = path.string();

        if (alpha)
        {
            data = stbi_load(path_str.c_str(), &m_width, &m_height, &m_channels, 4);
            m_channels = 4;
        }
        else
        {
            data = stbi_load(path_str.c_str(), &m_width, &m_height, &m_channels, 3);
            m_channels = 3;
        }

        if (!data)
        {
            LOG_ERROR("Texture not load. Path: {}", path_str);
            return false;
        }

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
            LOG_ERROR("Unsupported number of channels: {}", m_channels);
            stbi_image_free(data);
            data = nullptr;
            return false;
        }

        this->generate(m_width, m_height, data);
        LOG_DEBUG("Texture loaded: \"{}\".", path_str);
        stbi_image_free(data);
        return true;
    }
}