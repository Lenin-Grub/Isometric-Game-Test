#include <iostream>

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

    return true;
}

bool smpl::Window::close()
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

void smpl::Window::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

GLFWwindow& smpl::Window::getWindow() const
{
    assert(window && "Window is not created!");
    return *window;
}