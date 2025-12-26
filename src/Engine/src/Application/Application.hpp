#pragma once
#include <Window/Window.hpp>

namespace smpl 
{
    class Application
    {
    public:
        virtual void create() = 0;
        virtual void run()    = 0;

        bool isRunning() const
        {
            return m_running;
        };

        void close()
        {
            if (!m_running)
                m_window.close();
        };

        smpl::Window& getWindow() const
        {
            return m_window;
        };

    private:
        bool m_running = false;
        smpl::Window& m_window;
    };
}