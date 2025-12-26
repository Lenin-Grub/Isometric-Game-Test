#pragma once
#include <glm/glm.hpp>
#include <queue>

#include <Graphics/Color/Color.hpp>
#include <Window/VideoMode.hpp>
#include <Settings/Settings.hpp>

#include <Event/WindowEvents.hpp>
#include <Event/InputEvents.hpp>
#include <Event/Event.hpp>

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

        void raiseEvent(Core::Event& event);

        using EventCallbackFn = std::function<void(Core::Event&)>;
        EventCallbackFn EventCallback;

    private:
        GLFWwindow* m_window = nullptr;
        smpl::WindowSettings& m_window_settings;
    };
}