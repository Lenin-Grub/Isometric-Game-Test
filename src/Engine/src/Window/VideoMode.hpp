#pragma once
#include <stdio.h>
#include <vector>
#include <cstdint> 

namespace smpl
{
    class VideoMode
    {
    public:
        VideoMode();
        VideoMode(uint32_t width, uint32_t height);

        static VideoMode getDesktopMode();
        static std::vector<VideoMode> getFullscreenModes();

    public:
        uint32_t width  = 0;
        uint32_t height = 0;

        constexpr bool operator==(const VideoMode& other) const
        {
            return width == other.width && height == other.height;
        }

        constexpr bool operator!=(const VideoMode& other) const
        {
            return !(*this == other);
        }
    };
}