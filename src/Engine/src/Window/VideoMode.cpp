#include <stdafx.hpp>
#include <Window/VideoMode.hpp>
#include <GLFW/glfw3.h>

namespace smpl
{
    VideoMode::VideoMode()
        : width(640)
        , height(480)
    {
    }

    VideoMode::VideoMode(unsigned int width, unsigned int height)
        : width(width)
        , height(height)
    {
    }

    VideoMode VideoMode::getDesktopMode()
    {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        if (mode)
            return VideoMode(mode->width, mode->height);

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
            modes.emplace_back(vm.width, vm.height);
        }

        return modes;
    }
}
