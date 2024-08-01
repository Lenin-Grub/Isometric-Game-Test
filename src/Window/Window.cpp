#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Log.h"
#include "Window.h"

Window::Window()
    : m_widht(0)
    , m_height(0)
    , m_title("No Title")
    , m_window(nullptr)
{
}

Window::Window(const unsigned int widht, const unsigned int height, const std::string title)
    : m_widht  (widht)
    , m_height (height)
    , m_title  (title)
    , m_window (nullptr)
{
}

Window::~Window()
{
    close();
}

bool Window::create(const unsigned int widht, const unsigned int height, const std::string title)
{
    glfwInit();

    m_window = glfwCreateWindow(widht, height, title.c_str(), nullptr, nullptr);
    if (m_window == nullptr)
    {
        LOG_CRITICAL("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    gladLoadGL();
    return true;
}

bool Window::close()
{
    if (m_window)
    {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    else
    {
        return false;
    }
    glfwTerminate();

    return true;
}

void Window::clear()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(m_window);
}
