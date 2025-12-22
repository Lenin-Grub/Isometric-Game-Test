#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Log/Log.hpp>
#include <Window/Window.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <Settings/Settings.hpp>

namespace smpl
{
    Window::Window(smpl::Settings& settings)
        : m_window( nullptr )
        , m_window_settings( settings.window )
    {
    }

    Window::~Window()
    {
    }

    bool Window::create()
    {
        if (!glfwInit())
        {
            LOG_CRITICAL("GLFW init failed");
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        if (isResizable())
            glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        else
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        GLFWmonitor* monitor    = nullptr;
        const GLFWvidmode* mode = nullptr;

        if (m_window_settings.fullscreen)
        {
            monitor = glfwGetPrimaryMonitor();
            if (!monitor)
            {
                LOG_CRITICAL("No primary monitor found for fullscreen.");
                glfwTerminate();
                return false;
            }

            mode = glfwGetVideoMode(monitor);
            if (!mode)
            {
                LOG_CRITICAL("Failed to get video mode for fullscreen.");
                glfwTerminate();
                return false;
            }

            m_window_settings.mode.width  = mode->width;
            m_window_settings.mode.height = mode->height;
        }

        m_window = glfwCreateWindow( m_window_settings.mode.width, m_window_settings.mode.height, m_window_settings.title.c_str(), monitor, nullptr );

        if (!m_window)
        {
            LOG_CRITICAL("Window init failed");
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(m_window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LOG_CRITICAL("GLAD init failed");
            glfwDestroyWindow(m_window);
            glfwTerminate();
            return false;
        }

        glfwSetWindowUserPointer(m_window, this);
        glEnable(GL_DEPTH_TEST);

        return true;
    }

    bool Window::close()
    {
        if (!m_window)
        {
            LOG_CRITICAL("Attempted to close window, but no window exists.");
            return false;
        }

        glfwSetWindowShouldClose(m_window, GLFW_TRUE);

        return true;
    }

    void Window::clear()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Window::clear(const Color& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Window::display()
    {
        if (!m_window)
        {
            LOG_CRITICAL("Window is nulptr. Can`t display");
            return;
        }

        glfwSwapBuffers(m_window);
    }


    GLFWwindow& Window::getWindow() const
    {
        assert(m_window && "Window is not created!");
        return *m_window;
    }

    void Window::setFullscreen(bool fullscreen)
    {
        if (m_window_settings.fullscreen == fullscreen)
            return;

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        if (!monitor)
            return;

        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        if (!mode)
            return;

        m_window_settings.fullscreen = fullscreen;

        if (fullscreen)
        {
            glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        }
        else
        {
            int border_top, border_left, border_bottom, border_right;
            glfwGetWindowFrameSize(m_window, &border_left, &border_top, &border_right, &border_bottom);

            int windowWidth = m_window_settings.mode.width;
            int windowHeight = m_window_settings.mode.height;

            int windowX = (mode->width  - m_window_settings.mode.width)  / 2;
            int windowY = (mode->height - m_window_settings.mode.height) / 2;

            glfwSetWindowMonitor(m_window, nullptr, windowX, windowY,
                windowWidth, windowHeight, 0);
        }
    }

    void Window::setVideoMode(const VideoMode& mode, bool fullscreen)
    {
        if (!m_window)
        {
            LOG_CRITICAL("Cannot set mode: window not created.");
            return;
        }

        GLFWmonitor* monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;

        // fulscreen
        if (monitor)
        {
            const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);
            if (!vidMode)
            {
                LOG_CRITICAL("Failed to get video mode for monitor.");
                return;
            }

            glfwSetWindowMonitor(m_window, monitor, 0, 0, mode.width, mode.height, vidMode->refreshRate);
        }
        else
        {
            const GLFWvidmode* desktop = glfwGetVideoMode(glfwGetPrimaryMonitor());
            int x = (desktop->width - mode.width) / 2;
            int y = (desktop->height - mode.height) / 2;

            glfwSetWindowMonitor(m_window, nullptr, x, y, mode.width, mode.height, 0);
        }

        m_window_settings.mode       = mode;
        m_window_settings.fullscreen = fullscreen;
    }

    void Window::setVerticalSync(bool enable)
    {
        glfwSwapInterval(enable ? 1 : 0);
        m_window_settings.vertical_sync = enable;
    }

    bool Window::isVerticalSync() const
    {
        return m_window_settings.vertical_sync;
    }

    void Window::setResizable(bool enable)
    {
        m_window_settings.resizable = enable;
    }

    bool Window::isResizable() const
    {
        return m_window_settings.resizable;
    }

    const VideoMode& Window::getVideoMode()
    {
        return m_window_settings.mode;
    }

    VideoMode Window::getDesktopResolution()
    {
        if (!glfwInit())
            return smpl::VideoMode(640, 480);

        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        if (mode)
            return smpl::VideoMode(static_cast<uint32_t>(mode->width), static_cast<uint32_t>(mode->height));

        return smpl::VideoMode(640, 480);
    }

    bool Window::isOpen()
    {
        if (!m_window)
            return false;

        return !glfwWindowShouldClose(m_window);
    }

    bool Window::isFullscreen() const
    {
        return m_window_settings.fullscreen;
    }

    glm::vec2 Window::getCursorPos() const
    {
        double x_pos;
        double y_pos;
        glfwGetCursorPos(m_window, &x_pos, &y_pos);
        return { x_pos, y_pos };
    }
}