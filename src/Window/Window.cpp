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

bool Window::create(const unsigned int width, const unsigned int height, const std::string title)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (m_window == nullptr)
    {
        LOG_CRITICAL("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    // если включено, то при изменении экрана будет раст€гиватьс€ изображение
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

    gladLoadGL();
    return true;
}

bool Window::close()
{
    if (!glfwWindowShouldClose(m_window))
    {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
        glfwTerminate();
        return true;
    }
    else
    {
        return false;
    }
}

void Window::clear()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(m_window);
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
