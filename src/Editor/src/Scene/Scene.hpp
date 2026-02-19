#pragma once
#include <string>
#include <memory>
#include <array>

#include <Graphics/Shader/Shader.hpp>
#include <Graphics/VertexBuffer/VertexBuffer.hpp>
#include <Graphics/VertexArray/VertexArray.hpp>
#include <Graphics/Texture/Texture.hpp>
#include <Graphics/Sprite/Sprite.hpp>
#include <Graphics/Primitives/Cube/Cube.hpp>
#include <Graphics/Primitives/Grid/Grid.hpp>
#include <Camera/Camera.hpp>

namespace Editor
{
    class Scene
    {
    public:
        struct GlobalState
        {
            glm::vec3 camera_pos = { 0.f, -30.f, 15.f };
            glm::vec3 camera_rot = {-45.f, 90.f,  0.f };

            glm::vec3 scale      = { 1.f, 1.f, 1.f };
            glm::vec3 translate  = { 0.f, 0.f, 0.f };
            glm::vec3 rotate     = { 0.f, 0.f, 0.f };

            bool  isometric_mode = false;

            float delta_time = 0.0f;
            float last_frame = 0.0f;
            float angle      = 0.f;
            float fov        = 0.f;
            float zoom       = 0.f;

            smpl::Camera  camera { camera_pos, camera_rot };
        };

        Scene(const std::string& name);
        ~Scene();

        bool init();
        void update();
        void render();

        bool load();
        bool unload();

        bool isActive();
        void activate();

        smpl::Camera& getCamera() { return m_state.camera; }
        GlobalState&  getState()  { return m_state; }

    private:
        void initShaders();
        void initTextures();
        void initObjects();
        void renderCubes();

    private:
        std::string m_scene_name;
        bool        m_is_active;
        bool        m_is_loaded;

        // Rendering resources
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
        smpl::Cube                  m_cube;

        std::array<glm::vec3, 6>            m_positions;
        std::unique_ptr<smpl::VertexArray>  m_vao;
        std::unique_ptr<smpl::VertexBuffer> m_vbo;
        std::unique_ptr<smpl::IndexBuffer>  m_index_buffer;

        smpl::Texture2D m_first_texture2d;
        smpl::Texture2D m_second_texture2d;

        smpl::Texture m_texture1;
        smpl::Texture m_texture2;

        GlobalState m_state;

    };
}
