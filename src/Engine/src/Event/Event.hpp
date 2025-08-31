#pragma once

#include <Event/Keyboard.hpp>
#include <Event/Mouse.hpp>

namespace smpl
{
    struct KeyEvent 
    {
        //int key;
        Key::Code code;
        int scancode;
        int action;
        //int mods;
    };

    struct MouseButtonEvent 
    {
        Mouse::Button button;
        float x;
        float y;
        //int button;
        //int action;
        //int mods;
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
        double xoffset;
        double yoffset;
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

        TextInput,
        DragAndDrop,

        WindowClosed,
        WindowResized,
        WindowFocusLost,
        WindowFocusGained,
    };

    struct Event 
    {
        EventType type = EventType::None;

        union
        {
            KeyEvent         key;
            MouseButtonEvent mouseButton;
            MouseMoveEvent   mouseMove;
            WindowSizeEvent  windowSize;
            MouseScrollEvent mouseScroll;
        };
    };

}