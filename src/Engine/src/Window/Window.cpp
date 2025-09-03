#include <stdafx.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Log/Log.hpp>
#include <Window/Window.hpp>


smpl::Window::Window()
    : m_width  (0)
    , m_height (0)
    , m_title  ("No Title")
    , m_window (nullptr)
{
}

smpl::Window::Window(smpl::VideoMode& mode, const std::string& title)
    : m_width  (mode.width)
    , m_height (mode.height)
    , m_title  (title)
    , m_window (nullptr)
{
}

smpl::Window::~Window()
{
}

bool smpl::Window::create(smpl::VideoMode& mode, const std::string& title)
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

    glfwSetKeyCallback         (m_window, keyCallback);
    glfwSetMouseButtonCallback (m_window, mouseButtonCallback);
    glfwSetCursorPosCallback   (m_window, cursorPosCallback);
    glfwSetWindowCloseCallback (m_window, windowCloseCallback);
    glfwSetWindowSizeCallback  (m_window, windowSizeCallback);
    glfwSetScrollCallback      (m_window, windowScrollCallback);

    return true;
}

bool smpl::Window::close()
{
    if (!m_window)
    {
        LOG_CRITICAL("Attempted to close window, but no window exists.");
        return false;
    }

    //glfwDestroyWindow(window);
    //window = nullptr;
    //glfwTerminate();

    glfwSetWindowShouldClose(m_window, GLFW_TRUE);

    return true;
}

void smpl::Window::clear()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void smpl::Window::clear(const Color& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void smpl::Window::display()
{
    if (!m_window)
    {
        LOG_CRITICAL("Window is nulptr. Can`t display");
        return;
    }
 
    glfwSwapBuffers(m_window);
}


GLFWwindow& smpl::Window::getWindow() const
{
    assert(m_window && "Window is not created!");
    return *m_window;
}

bool smpl::Window::setFullscreen(bool fullscreen)
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

bool smpl::Window::setMode(const VideoMode& mode, bool fullscreen)
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
        int x = (desktop->width - mode.width)   / 2;
        int y = (desktop->height - mode.height) / 2;

        glfwSetWindowMonitor(m_window, nullptr, x, y, mode.width, mode.height, 0);
    }

    m_windowedMode = mode;
    m_width        = mode.width;
    m_height       = mode.height;
    m_fullscreen   = fullscreen;

    return true;
}

bool smpl::Window::pollEvent(smpl::Event& event)
{
    glfwPollEvents();

    if (m_event_queue.empty())
        return false;

    event = m_event_queue.front();
    m_event_queue.pop();
    return true;
}

bool smpl::Window::isOpen()
{
    if (!m_window)
        return false;

    return !glfwWindowShouldClose(m_window);
}

bool smpl::Window::isFullscreen() const
{
    return m_fullscreen;
}

void smpl::Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    Event event;
    event.type = (action == GLFW_PRESS) ? EventType::KeyPressed : EventType::KeyReleased;
    event.key.code = static_cast<Key::Code>(key);
    event.key.scancode = scancode;
    event.key.action = action;
    //event.key.mods = mods;
    win->m_event_queue.push(event);
}

void smpl::Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) 
{
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    Event event;
    event.type = (action == GLFW_PRESS) ? EventType::MouseButtonPressed : EventType::MouseButtonReleased;
    //event.mouseButton.button = button;
    event.mouseButton.button = static_cast<Mouse::Button>(button);
    //event.mouseButton.action = action;
    //event.mouseButton.mods = mods;
    win->m_event_queue.push(event);
}

void smpl::Window::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) 
{
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    Event event;
    event.type = EventType::MouseMoved;
    event.mouseMove.x = xpos;
    event.mouseMove.y = ypos;
    win->m_event_queue.push(event);
}

void smpl::Window::windowCloseCallback(GLFWwindow* window) 
{
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    Event event;
    event.type = EventType::WindowClosed;
    win->m_event_queue.push(event);
}

void smpl::Window::windowSizeCallback(GLFWwindow* window, int width, int height) 
{
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    Event event;
    event.type = EventType::WindowResized;
    event.windowSize.width = width;
    event.windowSize.height = height;
    win->m_event_queue.push(event);
}

void smpl::Window::windowScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    Event event;
    event.type = EventType::MouseScrolled;
    event.mouseScroll.xoffset = xoffset;
    event.mouseScroll.yoffset = yoffset;
    win->m_event_queue.push(event);
}