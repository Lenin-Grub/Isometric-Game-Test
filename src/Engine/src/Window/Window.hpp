#pragma once

#include <Graphics/Color/Color.hpp>
#include <Window/VideoMode.hpp>
#include <Settings/Settings.hpp>

#include <glm/glm.hpp>
#include <queue>

class GLFWwindow;

namespace smpl
{
    class Window
    {
    public:
        Window(smpl::Settings& settings);
        ~Window();

        bool create();

        bool close();

        bool isOpen();

        void clear();

        void clear(const Color& color);

        void display();

        GLFWwindow& getWindow() const;

        void setFullscreen(bool fullscreen);

        bool isFullscreen() const;

        void setVideoMode(const VideoMode& mode, bool fullscreen = false);

        void setVerticalSync(bool enable);

        bool isVerticalSync() const;

        void setResizable(bool enable);

        bool isResizable() const;

        const VideoMode& getVideoMode();

        static VideoMode getDesktopResolution();

        glm::vec2 getCursorPos() const;

    private:
        GLFWwindow* m_window = nullptr;
        smpl::WindowSettings& m_window_settings;
    };
}