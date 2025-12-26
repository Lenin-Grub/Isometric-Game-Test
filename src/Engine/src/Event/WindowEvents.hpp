#pragma once

#include <Event/Event.hpp>

#include <format>

namespace Core 
{
    class WindowClosedEvent 
        : public Event
    {
    public:
        WindowClosedEvent() 
        {
            // Do nothing
        }

        EVENT_CLASS_TYPE(WindowClose)
    };

    class WindowResizeEvent
        : public Event
    {
    public:
        WindowResizeEvent(uint32_t width, uint32_t height)
            : m_width(width)
            , m_height(height)
        {
            // Do nothing
        }

        inline uint32_t getWidth()  const { return m_width; }
        inline uint32_t getHeight() const { return m_height; }

        std::string toString() const override
        {
            return std::format("WindowResizeEvent: {}, {}", m_width, m_height);
        }

        EVENT_CLASS_TYPE(WindowResize)

        uint32_t m_width;
        uint32_t m_height;
    private:
    };
}
