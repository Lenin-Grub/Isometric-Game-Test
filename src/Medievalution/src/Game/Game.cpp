#include <Game/Game.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

#pragma region Call Backs
// TODO delete later. Unused
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
        LOG_DEBUG("Key pressed: {}", std::to_string(key));
}

void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    LOG_DEBUG("Mouse position is {} {}", xpos, ypos);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    LOG_DEBUG("Mouse button callback.");
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        LOG_DEBUG("Mouse button rights is pressed.");
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    LOG_DEBUG("Mouse scrolling.");
}

#pragma endregion

bool Game::create()
{
    if (!window.create(WIDTH, HEIGHT, "Medievalution"))
        return false;

    if(!initImGui(window))
        return false;

    if(!initImGuiFont())
        return false;

    if(!LogInfo::initLogger())
        return false;

    return true;
}

void Game::run()
{
    while (window.isOpen())
    {
        window.clear(color);

        init();
        input();
        update();
        draw();

        window.display();
    }

    close();
}

bool Game::init()
{
    if(!initBackEndImGui())
        return false;

    return true;
}

void Game::input()
{
    while (window.pollEvent(event))
    {
        if (event.type == smpl::EventType::WindowClosed)
        {
            window.close();
        }

        if (event.key.key == GLFW_KEY_ESCAPE)
        {
            window.close();
        }

        if (event.key.key == GLFW_KEY_SPACE)
        {
            LOG_DEBUG("Space key pressed!");
        }

        if (event.type == smpl::EventType::WindowResized)
        {
            LOG_DEBUG("Resized to : {0} {1}", event.windowSize.width, event.windowSize.height);
        }

        if (event.type == smpl::EventType::MouseButtonPressed)
        {
            LOG_DEBUG("Mouse ckicked!");
        }

    }
}

void Game::draw()
{
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
    ImGui::Render();
    drawImGuiGL();
}

void Game::update()
{

}

void Game::close()
{
    destroyImGui();
    window.close();
}
