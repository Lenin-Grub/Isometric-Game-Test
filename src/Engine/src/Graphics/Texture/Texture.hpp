#pragma once

namespace smpl
{
    class Texture
    {
    public:
        Texture();
        ~Texture();

        bool loadFromFile(const std::string& file_path);

        unsigned int getTextureID() const;

        int getHeight() const;
        int getWidth() const;
        int getChannels() const;

    private:

        unsigned int m_texture_id;
        int m_width;
        int m_height;
        int m_channels;
    };
}