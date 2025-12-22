#pragma once

#include <Log/Log.hpp>

#include <Graphics/Shader/Shader.hpp>
#include <Graphics/Texture/Texture.hpp>
#include <Graphics/Color/Color.hpp>
#include <Graphics/VertexBuffer/VertexBuffer.hpp>
#include <Graphics/VertexArray/VertexArray.hpp>
#include <Graphics/IndexBuffer/IndexBuffer.hpp>

#include <Window/Window.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <Settings/Settings.hpp>

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
};