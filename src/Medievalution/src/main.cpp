#include <Game/Game.hpp>
#include <Settings/Settings.hpp>
#include <GLFW/glfw3.h>

int main()
{
    smpl::Settings settings;
    if (!settings.loadFromFile("config/game_settings.json"))
        return 1;

    settings.window.mode       = smpl::Window::getDesktopResolution();
    settings.window.fullscreen = true;

    Game game(settings);
    game.create();
    game.run();
    return 0;
}