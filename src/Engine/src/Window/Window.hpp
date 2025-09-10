#pragma once

#include <Graphics/Color/Color.hpp>
#include <Event/Event.hpp>
#include <queue>
#include "VideoMode.hpp"
#include <glm/glm.hpp>

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

        bool setFullscreen(bool fullscreen);

        bool isFullscreen() const;

        bool setVideoMode(const VideoMode& mode, bool fullscreen = false);

        const VideoMode& getVideoMode();

        glm::vec2 getCursorPos() const;

    private:
        GLFWwindow*        m_window = nullptr;
        const std::string  m_title;
        unsigned int       m_width;
        unsigned int       m_height;

        VideoMode          m_video_mode;
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