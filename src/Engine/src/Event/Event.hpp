#pragma once

#include <string>
#include <functional>

namespace Core
{
    enum class EventType
    {
        None = 0,
        WindowClose,
        WindowResize,

        KeyPressed,
        KeyReleased,

        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled,
    };


#define EVENT_CLASS_TYPE(type)  static EventType getStaticType() { return EventType::type; }\
                                virtual EventType getEventType() const override { return getStaticType(); }\
                                virtual const char* getName() const override { return #type; }

    class Event
    {
    public:
        virtual ~Event() {}
        virtual EventType getEventType() const = 0;
        virtual const char* getName()    const = 0;
        virtual std::string toString()   const { return getName(); }

        bool handled = false;
    };

    class EventDispatcher
    {
        template<typename T>
        using EventFunction = std::function<bool(T&)>;

    public:
        EventDispatcher(Event& event)
            : m_event(event) 
        {
        }

        template<typename T>
        bool dispatch(EventFunction<T> func)
        {
            if (m_event.getEventType() == T::getStaticType() && !m_event.handled)
            {
                m_event.handled = func(*(T*)&m_event);
                return true;
            }
            return false;
        }

    private:
        Event& m_event;
    };
}
