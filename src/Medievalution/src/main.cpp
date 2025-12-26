#include <Game/Game.hpp>
#include <Settings/Settings.hpp>
#include <GLFW/glfw3.h>

int main()
{
    smpl::Settings settings;
    if (!settings.loadFromFile("config/game_settings.json"))
        return 1;

    Game game(settings);
    game.create();
    game.run();
    return 0;
}
