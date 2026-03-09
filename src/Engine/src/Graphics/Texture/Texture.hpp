#pragma once

#include <glad/glad.h>
#include <string>
#include <filesystem>

#include <Utils/NonCopyable.hpp>

namespace smpl
{
    class Texture2D
        : public smpl::NonCopyable
        , public smpl::NonMovable
    {
    public:
        Texture2D();
        ~Texture2D();

        void generate(int width, int height, unsigned char* data);
        void bind() const;

        bool loadTextureFromFile(const std::filesystem::path& path, bool alpha = true);

        unsigned int getID()       const;
        unsigned int getHeight()   const;
        unsigned int getWidth()    const;
        unsigned int getChannels() const;

    private:
        int m_width;
        int m_height;
        int m_channels;
        unsigned int m_id;
        unsigned int m_internal_format;
        unsigned int m_image_format;
        unsigned int m_wrap_s;
        unsigned int m_wrap_t;
        unsigned int m_filter_min;
        unsigned int m_filter_max;
    };
}