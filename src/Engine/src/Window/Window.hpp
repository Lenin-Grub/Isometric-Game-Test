#pragma once

#include <Graphics/Color/Color.hpp>
#include <Event/Event.hpp>
#include <queue>
#include "VideoMode.hpp"

class GLFWwindow;

namespace smpl
{
    class Window
    {
    public:
        Window();
        Window(smpl::VideoMode& mode, const std::string& title);
        ~Window();

        bool create(smpl::VideoMode& mode, const std::string& title);
        bool close();

        bool pollEvent(smpl::Event& event);
        bool isOpen();

        void clear();
        void clear(const Color& color);

        void display();

        GLFWwindow& getWindow() const;

        bool setFullscreen(bool fullscreen)
        {
            if (!m_window || m_fullscreen == fullscreen)
                return true;

            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            if (!monitor)
                return false;

            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            if (!mode)
                return false;

            if (fullscreen)
            {
                glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
                LOG_DEBUG("Fullscrean mod eanable");
            }
            else
            {
                int windowX = (mode->width - m_windowedMode.width) / 2;
                int windowY = (mode->height - m_windowedMode.height) / 2;

                glfwSetWindowMonitor(m_window, nullptr, windowX, windowY, m_windowedMode.width, m_windowedMode.height, 0);

                LOG_DEBUG("Fullscrean mod disable");
            }

            m_fullscreen = fullscreen;
            return true;
        }

    private:
        GLFWwindow*        m_window = nullptr;
        const std::string  m_title;
        const unsigned int m_width;
        const unsigned int m_height;

        VideoMode          m_windowedMode;     // Параметры оконного режима
        VideoMode          m_fullscreenMode;   // Режим для полноэкранного (обычно primary monitor)
        bool               m_fullscreen = false;

        std::queue<smpl::Event> m_event_queue;

        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        static void windowCloseCallback(GLFWwindow* window);
        static void windowSizeCallback(GLFWwindow* window, int width, int height);
        static void windowScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    };
}