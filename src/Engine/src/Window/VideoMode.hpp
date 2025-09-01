#pragma once

namespace smpl
{
	class VideoMode
	{
    public:

        VideoMode();
        VideoMode(unsigned int width, unsigned int height);

        static VideoMode getDesktopMode();
        static std::vector<VideoMode> getFullscreenModes();

    public:
        unsigned int width  = 0;
        unsigned int height = 0;

        bool operator == (const VideoMode& other) const
        {
            return *this == other;
        }

        bool operator != (const VideoMode& other) const
        {
            return *this != other;
        }
	};
}