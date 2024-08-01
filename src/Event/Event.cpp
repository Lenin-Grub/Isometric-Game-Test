#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Log.h"

#include "Event.h"

void Event::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        LOG_INFO("Input key");
    }
}