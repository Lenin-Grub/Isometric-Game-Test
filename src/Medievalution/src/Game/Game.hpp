#pragma once

#include <Log/Log.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Graphics/Shader/Shader.hpp>
#include <Graphics/Texture/Texture.hpp>
#include <Graphics/Color/Color.hpp>
#include <Graphics/Sprite/Sprite.hpp>
#include <Graphics/VertexBuffer/VertexBuffer.hpp>
#include <Graphics/VertexArray/VertexArray.hpp>
#include <Graphics/IndexBuffer/IndexBuffer.hpp>

#include <Window/Window.hpp>
#include <Settings/Settings.hpp>
#include <Camera/Camera.hpp>

#include <ECS/GameObject/GameObject.hpp>
#include <ECS/Components.hpp>
#include <Graphics/Primitives/Grid/Grid.hpp>

class Game
{
public:
    Game(smpl::Settings& settings);

    void create();
    void run();

private:

    bool init();
    void input();
    void draw();
    void update();

    void close();

    void initVideoModeList();
    void showVideoSettings();
    void updateImGuiDisplaySize();

private:
    smpl::Window m_window;
    smpl::Color  m_color{ 50,50,50 };

private:
    bool m_show_settings  = false;
    int  m_selected_index = 0;

    smpl::Settings&              m_settings;
    std::vector<smpl::VideoMode> m_available_modes;
    std::vector<std::string>     m_mode_labels;

private:
    smpl::ShaderProgram program;
    smpl::Shader fragment_shader;
    smpl::Shader vertex_shader;
    smpl::Sprite sprite;
    smpl::Texture2D texture;

    glm::vec3 camera_pos = { 0.f, 0.f, 25.f };
    glm::vec3 camera_rot = { -90.f, -90.f,  0.f };

    std::unique_ptr<smpl::Grid> m_grid;

    smpl::Camera camera{ camera_pos, camera_rot };

    smpl::ECS::GameObject m_game_obj;
    entt::registry m_scene;
};