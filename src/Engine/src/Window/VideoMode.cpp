#include <Window/VideoMode.hpp>
#include <GLFW/glfw3.h>

namespace smpl
{
    VideoMode::VideoMode()
        : width (640)
        , height(480)
    {
    }

    VideoMode::VideoMode(uint32_t width, uint32_t height)
        : width(width)
        , height(height)
    {
    }

    VideoMode VideoMode::getDesktopMode()
    {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        if (mode)
            return VideoMode((uint32_t)mode->width, (uint32_t)mode->height);

        return VideoMode(640, 480);
    }

    std::vector<smpl::VideoMode> VideoMode::getFullscreenModes()
    {
        std::vector<smpl::VideoMode> modes;

        int count = 0;
        GLFWmonitor* monitor           = glfwGetPrimaryMonitor();
        const GLFWvidmode* video_modes = glfwGetVideoModes(monitor, &count);

        for (int i = 0; i < count; ++i)
        {
            const auto& mode = video_modes[i];
            modes.emplace_back(mode.width, mode.height);
        }

        return modes;
    }
}
