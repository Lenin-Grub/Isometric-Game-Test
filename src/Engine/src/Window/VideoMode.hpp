#pragma once

#include <vector>
#include <cstdint> 

namespace smpl
{
    class VideoMode
    {
    public:
        VideoMode();
        VideoMode(std::uint32_t width, std::uint32_t height)
            : width(width), height(height)
        {}

        static VideoMode getDesktopMode();
        static std::vector<VideoMode> getFullscreenModes();

    public:
        std::uint32_t width;
        std::uint32_t height;

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