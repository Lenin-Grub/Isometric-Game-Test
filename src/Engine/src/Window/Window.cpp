#include <stdafx.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Log/Log.hpp>
#include <Window/Window.hpp>


smpl::Window::Window()
    : m_width(0)
    , m_height (0)
    , m_title  ("No Title")
    , window   (nullptr)
{
}

smpl::Window::Window(const unsigned int width, const unsigned int height, const std::string& title)
    : m_width  (width)
    , m_height (height)
    , m_title  (title)
    , window   (nullptr)
{
}

smpl::Window::~Window()
{
}

bool smpl::Window::create(const unsigned int width, const unsigned int height, const std::string& title)
{

    if (!glfwInit())
    {
        LOG_CRITICAL("GLFW init failed");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (!window)
    {
        LOG_CRITICAL("Window init failed");
        glfwDestroyWindow(window);
        window = nullptr;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_CRITICAL("GLAD init failed");
        glfwDestroyWindow(window);
        window = nullptr;
        glfwTerminate();
        return false;
    }

    glfwSetWindowUserPointer(window, this);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetWindowCloseCallback(window, windowCloseCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);

    return true;
}

bool smpl::Window::close()
{
    if (!window)
    {
        LOG_CRITICAL("Attempted to close window, but no window exists.");
        return false;
    }

    //glfwDestroyWindow(window);
    //window = nullptr;
    //glfwTerminate();

    glfwSetWindowShouldClose(window, GLFW_TRUE);

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
    if (!window)
    {
        LOG_CRITICAL("Window is nulptr. Can`t display");
        return;
    }
 
    glfwSwapBuffers(window);
}

//void smpl::Window::processInput(GLFWwindow* window)
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//}

GLFWwindow& smpl::Window::getWindow() const
{
    assert(window && "Window is not created!");
    return *window;
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
    if (!window)
        return false;

    return !glfwWindowShouldClose(window);
}

void smpl::Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    Event event;
    event.type = (action == GLFW_PRESS) ? EventType::KeyPressed : EventType::KeyReleased;
    event.key.key = key;
    event.key.scancode = scancode;
    event.key.action = action;
    event.key.mods = mods;
    win->m_event_queue.push(event);
}

void smpl::Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) 
{
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    Event event;
    event.type = (action == GLFW_PRESS) ? EventType::MouseButtonPressed : EventType::MouseButtonReleased;
    event.mouseButton.button = button;
    event.mouseButton.action = action;
    event.mouseButton.mods = mods;
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

void smpl::Window::scrollCallback(GLFWwindow* window, float xoffset, float yoffset)
{
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (!win) return;
    Event event;
    event.type = EventType::MouseScrolled;
    event.mouseScroll.xoffset = xoffset;
    event.mouseScroll.yoffset = yoffset;
    win->m_event_queue.push(event);
}