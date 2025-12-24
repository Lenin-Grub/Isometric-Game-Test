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

    smpl::Camera camera;

    glm::vec3 pos{25,2.5,2.5};
    glm::vec3 rot{90,0,270};
    glm::vec3 size{10,10,10};

    float m_animation_timer    = 0.0f;
    float m_animation_interval = 0.1f;
    int m_current_frame        = 0;
    int m_max_frames           = 11;
    int m_frame_width          = 64;
};