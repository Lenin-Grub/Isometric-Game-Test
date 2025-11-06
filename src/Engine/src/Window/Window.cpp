#include <stdafx.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Log/Log.hpp>
#include <Window/Window.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace smpl
{

    Window::Window()
        : m_width(0)
        , m_height(0)
        , m_title("No Title")
        , m_window(nullptr)
    {
    }

    Window::Window(smpl::VideoMode& mode, const std::string& title)
        : m_width(mode.width)
        , m_height(mode.height)
        , m_title(title)
        , m_window(nullptr)
    {
    }

    Window::~Window()
    {
    }

    bool Window::create(smpl::VideoMode& mode, const std::string& title)
    {

        if (!glfwInit())
        {
            LOG_CRITICAL("GLFW init failed");
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow(mode.width, mode.height, title.c_str(), nullptr, nullptr);
        setVideoMode(mode);

        if (!m_window)
        {
            LOG_CRITICAL("Window init failed");
            glfwDestroyWindow(m_window);
            m_window = nullptr;
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(m_window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            LOG_CRITICAL("GLAD init failed");
            glfwDestroyWindow(m_window);
            m_window = nullptr;
            glfwTerminate();
            return false;
        }

        glfwSetWindowUserPointer   (m_window, this);

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

    bool Window::setFullscreen(bool fullscreen)
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
            int windowX = (mode->width  - m_video_mode.width) / 2;
            int windowY = (mode->height - m_video_mode.height) / 2;

            glfwSetWindowMonitor(m_window, nullptr, windowX, windowY, m_video_mode.width, m_video_mode.height, 0);

            LOG_DEBUG("Fullscrean mod disable");
        }

        m_fullscreen = fullscreen;
        return true;
    }

    bool Window::setVideoMode(const VideoMode& mode, bool fullscreen)
    {
        if (!m_window)
        {
            LOG_CRITICAL("Cannot set mode: window not created.");
            return false;
        }

        GLFWmonitor* monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;

        // fulscreen
        if (monitor)
        {
            const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);
            if (!vidMode)
            {
                LOG_CRITICAL("Failed to get video mode for monitor.");
                return false;
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

        m_video_mode = mode;
        m_width      = mode.width;
        m_height     = mode.height;
        m_fullscreen = fullscreen;

        return true;
    }

    void Window::setVerticalSync(bool enable)
    {
        glfwSwapInterval(enable ? 1 : 0);
        m_vertical_synch = enable;
    }

    bool Window::isVerticalSync() const
    {
        return m_vertical_synch;
    }

    const VideoMode& Window::getVideoMode()
    {
        return m_video_mode;
    }

    bool Window::isOpen()
    {
        if (!m_window)
            return false;

        return !glfwWindowShouldClose(m_window);
    }

    bool Window::isFullscreen() const
    {
        return m_fullscreen;
    }

    glm::vec2 Window::getCursorPos() const
    {
        double x_pos;
        double y_pos;
        glfwGetCursorPos(m_window, &x_pos, &y_pos);
        return { x_pos, y_pos };
    }
}