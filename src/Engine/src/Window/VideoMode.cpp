#include <stdafx.hpp>
#include <Window/VideoMode.hpp>
#include <GLFW/glfw3.h>

namespace smpl
{
    VideoMode::VideoMode()
        : width (640)
        , height(480)
    {
    }

    VideoMode::VideoMode(std::uint32_t width, std::uint32_t height)
        : width(width)
        , height(height)
    {
    }

    VideoMode VideoMode::getDesktopMode()
    {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        if (mode)
            return VideoMode(static_cast<std::uint32_t>(mode->width),
                             static_cast<std::uint32_t>(mode->height));

        return VideoMode(640, 480);
    }

    std::vector<smpl::VideoMode> VideoMode::getFullscreenModes()
    {
        std::vector<smpl::VideoMode> modes;

        int count;
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* vidModes = glfwGetVideoModes(monitor, &count);

        for (int i = 0; i < count; ++i)
        {
            const auto& vm = vidModes[i];
            modes.emplace_back(static_cast<std::uint32_t>(vm.width),
                               static_cast<std::uint32_t>(vm.height));
        }

        return modes;
    }
}
