#include <Game/Game.hpp>
#include <GLFW/glfw3.h>

#include <Window/VideoMode.hpp>
#include <ImGui/SetImGui.hpp>
#include <Event/Input.hpp>

#include <iostream>
#include <set>
#include <algorithm>

bool Game::create()
{
    smpl::VideoMode mode{ m_width , m_height };

    if (!m_window.create(mode, "Medievalution"))
        return false;

    if(!smpl::Gui::initImGui(m_window))
        return false;

    if(!smpl::Gui::initImGuiFont())
        return false;

    if(!LogInfo::initLogger())
        return false;

    if (!smpl::Input::init(m_window))
        return false;

    return true;
}

void Game::run()
{
    smpl::VideoMode modes;

    while (m_window.isOpen())
    {
        m_window.clear(m_color);

        init();
        input();
        update();
        draw();

        m_window.display();
    }

    close();
}

bool Game::init()
{
    if(!smpl::Gui::initBackEndImGui())
        return false;

    return true;
}

void Game::input()
{
    glfwPollEvents();

    if (smpl::Input::isKeyPressed(smpl::Key::Code::Escape))
    {
        m_window.close();
    }

    if (smpl::Input::isKeyPressed(smpl::Key::Code::Space))
    {
        LOG_DEBUG("Space key pressed!");
    }

    if (smpl::Input::isKeyReleased(smpl::Key::Code::Space))
    {
        LOG_INFO("Space key relesed!");
    }

    if (smpl::Input::isMousePressed(smpl::Mouse::Button::Left))
    {
        LOG_DEBUG("Mouse left ckicked!");
    }

    if (smpl::Input::isMouseReleased(smpl::Mouse::Button::Left))
    {
        LOG_INFO("Mouse left relesed!");
    }

    if (smpl::Input::isMouseScrolled(smpl::Mouse::Scroll::Up))
    {
        LOG_DEBUG("Mouse scrolled up!");
    }
    if (smpl::Input::isMouseScrolled(smpl::Mouse::Scroll::Down))
    {
        LOG_DEBUG("Mouse scrolled down!");
    }
}

void Game::draw()
{
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
    showVideoSettings();

    smpl::Gui::drawImGuiGL();
}

void Game::update()
{

}

void Game::close()
{
    smpl::Gui::destroyImGui();
    m_window.close();
}

#pragma region Settings

void Game::initVideoModeList()
{
    if (!m_available_modes.empty())
        return;

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

    m_available_modes = std::move(unique);

    m_mode_labels.clear();

    for (const auto& mode : m_available_modes)
    {
        m_mode_labels.push_back(std::to_string(mode.width) + "x" + std::to_string(mode.height));
    }

    m_selected_index = 0;
}

void Game::showVideoSettings()
{
    initVideoModeList();

    ImGui::Begin("Settings", &m_show_settings);

    if (ImGui::BeginCombo("Resolution", m_mode_labels[m_selected_index].c_str()))
    {
        for (int i = 0; i < m_mode_labels.size(); i++)
        {
            bool selected = (i == m_selected_index);
            if (ImGui::Selectable(m_mode_labels[i].c_str(), selected))
            {
                m_selected_index = i;
            }
            if (selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    ImGui::Checkbox("Fullscreen", &m_fullscreen);

    if (ImGui::Button("Applay"))
    {
        const smpl::VideoMode& mode = m_available_modes[m_selected_index];
        m_window.setVideoMode(mode, m_fullscreen);
        LOG_INFO("Resolution: {}x{} | Fullscreen: {}", mode.width, mode.height, m_fullscreen);
    }

    ImGui::End();
}

#pragma endregion
