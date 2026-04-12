#include <Game/Game.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Window/VideoMode.hpp>
#include <ImGui/SetImGui.hpp>
#include <Event/Input.hpp>

#include <iostream>
#include <set>
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>


Game::Game(smpl::Settings& settings)
    : m_settings{ settings }
    , m_window  { settings }
    , m_game_obj{ m_scene  }
{
    // Do nothing
};

void Game::create()
{
    if (!m_window.create())
        return;

    if(!smpl::Gui::initImGui(m_window))
        return;

    if(!smpl::Gui::initImGuiFont())
        return;

    if(!LogInfo::initLogger())
        return;

    if (!smpl::Input::init(m_window))
        return;

    if(!init())
        return;
}

void Game::run()
{
    while (m_window.isOpen())
    {
        m_window.clear(m_color);

        input();
        update();
        draw();

        m_window.display();
    }

    close();
}

bool Game::init()
{
    m_window.setIcon("res/icon.png");

    camera.setProjection(smpl::Camera::Projection::Orthographic);

    if(!smpl::Gui::initBackEndImGui())
        return false;

    m_game_obj.create();
    m_game_obj.addComponent<smpl::ECS::Components::Color>();
    m_game_obj.addComponent<smpl::ECS::Components::Transform>(
        glm::vec3( 0.0f,  0.0f,  1.0f), // position
        glm::vec3( 0.0f,  0.0f, 90.0f), // rotation  
        glm::vec3(10.0f, 10.0f, 0.0f)   // scale 
    );

    texture.loadTextureFromFile("res/Spearman.png");
    sprite.init();

    // Grid
    m_grid = std::make_unique<smpl::Grid>();
    m_grid->init();
    return true;
}

void Game::input()
{
    glfwPollEvents();

    if (smpl::Input::isKeyPressed(smpl::Key::Code::Escape))
        m_window.close();
}

void Game::draw()
{
    m_grid->draw(camera);
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
    showVideoSettings();

    ImGui::Begin("Entities");

    auto& color = m_game_obj.getComponent<smpl::ECS::Components::Color>();

    ImGui::ColorPicker4("Sprite color", glm::value_ptr(color.value));
    ImGui::End();

    sprite.setColor(color.value);

    auto& transform = m_game_obj.getComponent<smpl::ECS::Components::Transform>();

    sprite.draw(texture, camera);

    sprite.setPosition(transform.position);
    sprite.setRotation(transform.rotation);
    sprite.setColor(color.value);
    sprite.setScale(transform.scale);

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

    ImGui::Checkbox("Fullscreen", &m_settings.window.fullscreen);

    if (ImGui::Button("Apply"))
    {
        const smpl::VideoMode& mode = m_available_modes[m_selected_index];
        m_window.setVideoMode(mode, m_settings.window.fullscreen);
        LOG_INFO("Resolution: {}x{} | Fullscreen: {}", (int)mode.width, (int)mode.height, m_settings.window.fullscreen);
        updateImGuiDisplaySize();
    }

    auto& transform = m_game_obj.getComponent<smpl::ECS::Components::Transform>();

    ImGui::SliderFloat3("Scale",     glm::value_ptr(transform.scale),       0.5f,  50.0f);
    ImGui::SliderFloat3("Translate", glm::value_ptr(transform.position), -100.0f, 100.0f);
    ImGui::SliderFloat3("Rotate",    glm::value_ptr(transform.rotation),    0.0f, 360.0f);

    ImGui::Text("FPS: %.2f", m_window.getFPS());

    ImGui::End();
}

void Game::updateImGuiDisplaySize()
{
    int display_w  = m_window.getVideoMode().width;
    int display_h  = m_window.getVideoMode().height;
    ImGuiIO& io    = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)display_w, (float)display_h);
}

#pragma endregion
