#include <Game/Game.hpp>
#include <GLFW/glfw3.h>

#include <Window/VideoMode.hpp>
#include <ImGui/SetImGui.hpp>

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
    while (m_window.pollEvent(m_event))
    {
        if (m_event.type == smpl::EventType::WindowClosed)
        {
            m_window.close();
        }

        if (m_event.key.code == smpl::Key::Code::Escape)
        {
            m_window.close();
        }

        if (m_event.key.action == GLFW_RELEASE)
        {
            if (m_event.key.code == smpl::Key::Code::Space)
            {
                LOG_DEBUG("Space key relesed!");
            }
        }

        if (m_event.type == smpl::EventType::WindowResized)
        {
            LOG_DEBUG("Resized to : {0} {1}", m_event.windowSize.width, m_event.windowSize.height);
        }

        if (m_event.type == smpl::EventType::MouseButtonPressed)
        {
            if (m_event.mouseButton.button == smpl::Mouse::Left)
            {
                LOG_DEBUG("Mouse left ckicked!");
            }
        }

        if (m_event.type == smpl::EventType::MouseScrolled)
        {
            LOG_DEBUG("Mouse scrolled!");
        }

        //not work yet
        //if (m_event.type == smpl::EventType::KeyPressed)
        //{
        //    if (m_event.key.code == smpl::Key::Code::A && m_event.key.code == smpl::Key::Code::LShift && m_event.key.code == smpl::Key::Code::LCtrl)
        //    {
        //        LOG_DEBUG("Ctrl+Shift+A");
        //    }
        //}

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
