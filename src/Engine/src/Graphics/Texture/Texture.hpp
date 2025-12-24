#pragma once

#include <glad/glad.h>
#include <string>
#include <filesystem>

namespace smpl
{
    class Texture
    {
    public:
        Texture();
        ~Texture();

        bool loadFromFile(const std::string& filepath);

        unsigned int getTextureID() const;

        int getHeight()   const;
        int getWidth()    const;
        int getChannels() const;

    private:
        unsigned int m_texture_id;
        int m_width;
        int m_height;
        int m_channels;
    };

    class Texture2D
    {
    public:
        Texture2D();

        void generate(int width, int height, unsigned char* data);
        void bind() const;

        static Texture2D loadTextureFromFile(const std::filesystem::path& path, bool alpha = true);

        unsigned int getID()       const;
        unsigned int getHeight()   const;
        unsigned int getWidth()    const;
        unsigned int getChannels() const;

    private:
        unsigned int m_id;
        unsigned int m_width;
        unsigned int m_height;
        unsigned int m_channels;
        unsigned int m_internal_format;
        unsigned int m_image_format;
        unsigned int m_wrap_s;
        unsigned int m_wrap_t;
        unsigned int m_filter_min;
        unsigned int m_filter_max;
    };
}