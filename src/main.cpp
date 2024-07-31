#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Window/Window.h"

const unsigned int WIDTH  = 800;
const unsigned int HEIGHT = 600;

int main()
{
    Window window;
    window.create(WIDTH, HEIGHT, "Test");

    while (!glfwWindowShouldClose(window.m_window))
    {
        window.clear();
        glfwPollEvents();
    }

    window.close();
    return 0;
}