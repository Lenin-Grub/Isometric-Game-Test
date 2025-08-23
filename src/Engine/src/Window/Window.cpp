#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Log/Log.hpp>
#include <Window/Window.hpp>

Window::Window()
    : m_widht  (0)
    , m_height (0)
    , m_title  ("No Title")
    , window   (nullptr)
{
}

Window::Window(const unsigned int widht, const unsigned int height, const std::string& title)
    : m_widht  (widht)
    , m_height (height)
    , m_title  (title)
    , window   (nullptr)
{
}

Window::~Window()
{
}

bool Window::create(const unsigned int widht, const unsigned int height, const std::string& title)
{
    if (!glfwInit())
    {
        LOG_CRITICAL("GLFW init failed");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(widht, height, title.c_str(), nullptr, nullptr);

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

    return true;
}

bool Window::close()
{
    if (!window)
    {
        LOG_CRITICAL("Attempted to close window, but no window exists.");
        return false;
    }

    glfwDestroyWindow(window);
    window = nullptr;
    glfwTerminate();
    return true;
}

void Window::clear()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::clear(const Color& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::display()
{
    if (!window)
    {
        LOG_CRITICAL("Window is nulptr. Can`t display");
        return;
    }
 
    glfwSwapBuffers(window);
}
