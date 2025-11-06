#pragma once

#include <Graphics/Color/Color.hpp>
#include <Window/VideoMode.hpp>

#include <glm/glm.hpp>
#include <queue>

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

        bool isOpen();

        void clear();
        void clear(const Color& color);

        void display();

        GLFWwindow& getWindow() const;

        bool setFullscreen(bool fullscreen);

        bool isFullscreen() const;

        bool setVideoMode(const VideoMode& mode, bool fullscreen = false);

        void setVerticalSync(bool enable);

        bool isVerticalSync() const;

        const VideoMode& getVideoMode();

        glm::vec2 getCursorPos() const;

    private:
        GLFWwindow*        m_window = nullptr;
        const std::string  m_title;
        unsigned int       m_width;
        unsigned int       m_height;

        VideoMode          m_video_mode;
        bool               m_fullscreen     = false;
        bool               m_vertical_synch = false;
    };
}