#pragma once

#include <Event/Event.hpp>

#include <format>

namespace Core 
{
    //
    // Key Events
    //

    class KeyEvent
        : public Event
    {
    public:
        inline int getKeyCode() const { return m_KeyCode; }

    protected:
        KeyEvent(int keycode)
            : m_KeyCode(keycode)
        {
            // Do nothing
        }

        int m_KeyCode;
    };

    class KeyPressedEvent
        : public KeyEvent
    {
    public:
        KeyPressedEvent(int keycode, bool isRepeat)
            : KeyEvent(keycode)
            , m_IsRepeat(isRepeat) 
        {
            // Do nothing
        }

        inline bool isRepeat() const { return m_IsRepeat; }

        std::string toString() const override
        {
            return std::format("KeyPressedEvent: {} (repeat={})", m_KeyCode, m_IsRepeat);
        }

        EVENT_CLASS_TYPE(KeyPressed)

    private:
        bool m_IsRepeat;
    };

    class KeyReleasedEvent 
        : public KeyEvent
    {
    public:
        KeyReleasedEvent(int keycode)
            : KeyEvent(keycode) 
        {
            // Do nothing
        }

        std::string toString() const override
        {
            return std::format("KeyReleasedEvent: {}", m_KeyCode);
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

    //
    // Mouse Events
    //

    class MouseMovedEvent 
        : public Event
    {
    public:
        MouseMovedEvent(double x, double y)
            : m_pos_x(x)
            , m_pos_y(y) 
        {
            // Do nothing
        }

        inline double getX() const { return m_pos_x; }
        inline double getY() const { return m_pos_y; }

        std::string toString() const override
        {
            return std::format("MouseMovedEvent: {}, {}", m_pos_x, m_pos_y);
        }

        EVENT_CLASS_TYPE(MouseMoved)

    private:
        double m_pos_x;
        double m_pos_y;
    };

    class MouseScrolledEvent 
        : public Event
    {
    public:
        MouseScrolledEvent(double xOffset, double yOffset)
            : m_offset_x(xOffset)
            , m_offeset_y(yOffset) 
        {
            // Do nothing
        }

        inline double getXOffset() const { return m_offset_x; }
        inline double getYOffset() const { return m_offeset_y; }

        std::string toString() const override
        {
            return std::format("MouseScrolledEvent: {}, {}", m_offset_x, m_offeset_y);
        }

        EVENT_CLASS_TYPE(MouseScrolled)
    private:
        double m_offset_x;
        double m_offeset_y;
    };

    class MouseButtonEvent 
        : public Event
    {
    public:
        inline int getMouseButton() const { return m_button; }
    protected:
        MouseButtonEvent(int button)
            : m_button(button) 
        {
            // Do nothing
        }

        int m_button;
    };

    class MouseButtonPressedEvent 
        : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(int button)
            : MouseButtonEvent(button) 
        {
            // Do nothing
        }

        std::string toString() const override
        {
            return std::format("MouseButtonPressedEvent: {}", m_button);
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    class MouseButtonReleasedEvent 
        : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(int button)
            : MouseButtonEvent(button) 
        {
            // Do nothing
        }

        std::string toString() const override
        {
            return std::format("MouseButtonReleasedEvent: {}", m_button);
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };
}
