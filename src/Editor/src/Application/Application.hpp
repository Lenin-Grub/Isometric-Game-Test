#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <array>
#include <set>
#include <memory>

#include <Graphics/Shader/Shader.hpp>
#include <Graphics/VertexBuffer/VertexBuffer.hpp>
#include <Graphics/VertexArray/VertexArray.hpp>
#include <Graphics/Texture/Texture.hpp>
#include <Graphics/Sprite/Sprite.hpp>
#include <Graphics/Primitives/Cube/Cube.hpp>
#include <Graphics/Primitives/Grid/Grid.hpp>

#include <Window/Window.hpp>
#include <Event/Input.hpp>
#include <Camera/Camera.hpp>
#include <ImGui/SetImGui.hpp>
#include <Log/Log.hpp>
#include <Settings/Settings.hpp>

namespace EDITOR
{    class Application
    {
    public:
        Application();
        ~Application() = default;
        bool init();
        void run();

    private:
        void initShaders();
        void initTextures();
        void initObjects();
        void initGui();

        void setupEventCallbacks();

        void render();
        void renderCubes();
        
    private:
        smpl::Settings m_settings;
        std::unique_ptr<smpl::Window> m_window;

        smpl::Shader m_vertex_shader;
        smpl::Shader m_fragment_shader;
        smpl::ShaderProgram m_shader_cube;

        smpl::Shader m_vertex_shader2;
        smpl::Shader m_fragment_shader2;
        smpl::ShaderProgram m_shader_grid;

        smpl::Shader m_vertex_shader3;
        smpl::Shader m_fragment_shader3;
        smpl::ShaderProgram m_shader_plane;

        smpl::Sprite m_sprite1;
        smpl::Sprite m_sprite2;
        smpl::Sprite m_sprite3;

        std::unique_ptr<smpl::Grid> m_grid;
        smpl::Cube m_cube;

        std::array<glm::vec3, 6> m_positions;
        std::unique_ptr<smpl::VertexArray> m_vao;
        std::unique_ptr<smpl::VertexBuffer> m_vbo;
        std::unique_ptr<smpl::IndexBuffer> m_index_buffer;

        smpl::Texture2D m_first_texture2d;
        smpl::Texture2D m_second_texture2d;

    struct GlobalState
    {
        glm::vec3 camera_pos = { 0.f, -30.f, 15.f };
        glm::vec3 camera_rot = {-45.f, 90.f,  0.f };

        glm::vec3 scale      = { 1.f, 1.f, 1.f };
        glm::vec3 translate  = { 0.f, 0.f, 0.f };
        glm::vec3 rotate     = { 0.f, 0.f, 0.f };

        glm::vec3 saved_perspective_pos = camera_pos;
        glm::vec3 saved_perspective_rot = camera_rot;

        bool  isometric_mode = false;

        float delta_time = 0.0f;
        float last_frame = 0.0f;
        float angle      = 0.f;
        float fov        = 0.f;
        float zoom       = 0.f;

        smpl::Camera  camera { camera_pos, camera_rot };
        smpl::Texture texture1;
        smpl::Texture texture2;
    };
        GlobalState m_state;

    };
}