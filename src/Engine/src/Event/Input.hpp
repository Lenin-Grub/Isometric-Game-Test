#pragma once
#include <Event/Keyboard.hpp>
#include <Event/Mouse.hpp>
#include <Window/Window.hpp>
#include <Log/Log.hpp>

namespace smpl
{
    class Input
    {
    public:
        static bool init(smpl::Window& window);

        static bool isKeyPressed(smpl::Key::Code key);

        static bool isKeyReleased(smpl::Key::Code key);

        static bool isMousePressed(smpl::Mouse::Button button);

        static bool isMouseReleased(smpl::Mouse::Button button);

        static bool isMouseScrolled(smpl::Mouse::Scroll direction);

        static const float getScrollOffsetX();

        static const float getScrollOffsetY();

    private:
        static GLFWwindow* m_window;
        static inline std::unordered_map<int, bool> m_previous_state = {};
    };
}