#include <Game/Game.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Window/VideoMode.hpp>
#include <set>
#include <algorithm>

#pragma region Settings

static bool s_showSettings = false;
static bool s_fullscreen = false;
static int s_selectedIndex = 0;
static std::vector<smpl::VideoMode> s_availableModes;
static std::vector<std::string> s_modeLabels;

void initVideoModeList()
{
    if (!s_availableModes.empty()) return;

    auto modes = smpl::VideoMode::getFullscreenModes();

    std::set<std::pair<int, int>> seen;
    std::vector<smpl::VideoMode> unique;

    for (const auto& mode : modes)
    {
        auto key = std::make_pair(mode.width, mode.height);
        if (seen.insert(key).second)
        {
            unique.push_back(mode);
        }
    }

    std::sort(unique.begin(), unique.end(), [](const smpl::VideoMode& a, const smpl::VideoMode& b)
        { return (a.width * a.height) > (b.width * b.height); });

    s_availableModes = std::move(unique);

    s_modeLabels.clear();
    for (const auto& mode : s_availableModes)
    {
        s_modeLabels.push_back(std::to_string(mode.width) + "x" + std::to_string(mode.height));
    }

    s_selectedIndex = 0;
}

void Game::showVideoSettings()
{
    initVideoModeList();

    ImGui::Begin("Settings", &s_showSettings);

    if (ImGui::BeginCombo("Resolution", s_modeLabels[s_selectedIndex].c_str()))
    {
        for (int i = 0; i < s_modeLabels.size(); i++)
        {
            bool selected = (i == s_selectedIndex);
            if (ImGui::Selectable(s_modeLabels[i].c_str(), selected))
            {
                s_selectedIndex = i;
            }
            if (selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    ImGui::Checkbox("Fullscreen", &s_fullscreen);

    if (ImGui::Button("Applay"))
    {
        const smpl::VideoMode& mode = s_availableModes[s_selectedIndex];
        window.setMode(mode, s_fullscreen);
        LOG_INFO("Resolution: {}x{} | Fullscreen: {}", mode.width, mode.height, s_fullscreen);
    }

    ImGui::End();
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
    showVideoSettings();
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

