#include <Game/Game.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Window/VideoMode.hpp>


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
    smpl::VideoMode mode{ WIDTH , HEIGHT };

    if (!window.create(mode, "Medievalution"))
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
    smpl::VideoMode modes;

    for (auto i : modes.getFullscreenModes())
    {
        auto res = i;
        LOG_INFO("Video mode: {0},{1}", res.width, res.height);
    }


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

        if (event.key.code == smpl::Key::Code::Escape)
        {
            window.close();
        }

        if (event.key.action == GLFW_RELEASE)
        {
            if (event.key.code == smpl::Key::Code::Space)
            {
                LOG_DEBUG("Space key relesed!");
            }
        }

        if (event.type == smpl::EventType::WindowResized)
        {
            LOG_DEBUG("Resized to : {0} {1}", event.windowSize.width, event.windowSize.height);
        }

        if (event.type == smpl::EventType::MouseButtonPressed)
        {
            if (event.mouseButton.button == smpl::Mouse::Left)
            {
                LOG_DEBUG("Mouse left ckicked!");
            }
        }

        if (event.type == smpl::EventType::MouseScrolled)
        {
            LOG_DEBUG("Mouse scrolled!");
        }

        //not work yet
        if (event.type == smpl::EventType::KeyPressed)
        {
            if (event.key.code == smpl::Key::Code::A && event.key.code == smpl::Key::Code::LShift && event.key.code == smpl::Key::Code::LCtrl)
            {
                LOG_DEBUG("Ctrl+Shift+A");
            }
        }

    }
}

void Game::draw()
{
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();

    static bool fullscrean;

    ImGui::Begin("Settings");
    
    ImGui::Checkbox("Fullscrean", &fullscrean);
    window.setFullscreen(fullscrean);

    ImGui::End();

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
