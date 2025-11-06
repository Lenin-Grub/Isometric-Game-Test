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

class Game
{
public:
    Game()  = default;
    ~Game() = default;

    bool create();
    void run();

private:

    bool init();
    void input();
    void draw();
    void update();

    void close();

    void initVideoModeList();
    void showVideoSettings();

private:
    unsigned int m_width  = 2560;
    unsigned int m_height = 1600;

    smpl::Window m_window;
    smpl::Color  m_color{ 50,50,50 };

private:
    bool m_show_settings  = false;
    bool m_fullscreen     = false;
    int  m_selected_index = 0;
    std::vector<smpl::VideoMode> m_available_modes;
    std::vector<std::string> m_mode_labels;
};