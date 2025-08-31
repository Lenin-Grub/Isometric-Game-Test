#pragma once

#include <Event/Keyboard.hpp>
#include <Event/Mouse.hpp>

namespace smpl
{
    struct KeyEvent 
    {
        int key;
        int scancode;
        int action;
        int mods;
    };

    struct MouseClickEvent 
    {
        int button;
        int action;
        int mods;
    };

    struct MouseMoveEvent 
    {
        float x;
        float y;
    };

    struct WindowSizeEvent
    {
        unsigned int width;
        unsigned int height;
    };

    struct MouseScrollEvent 
    {
        float xoffset;
        float yoffset;
    };

    enum class EventType 
    {
        None = 0,

        KeyPressed,
        KeyReleased,

        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled,

        WindowClosed,
        WindowResized,
    };

    struct Event 
    {
        EventType type = EventType::None;

        union
        {
            KeyEvent         key;
            MouseClickEvent  mouseButton;
            MouseMoveEvent   mouseMove;
            WindowSizeEvent  windowSize;
            MouseScrollEvent mouseScroll;
        };
    };

}