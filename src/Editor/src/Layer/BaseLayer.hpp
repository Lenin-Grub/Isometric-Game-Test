#pragma once

#include <Layer/ILayer.hpp>

#include <Graphics/Shader/Shader.hpp>
#include <Graphics/VertexBuffer/VertexBuffer.hpp>
#include <Graphics/VertexArray/VertexArray.hpp>
#include <Graphics/Texture/Texture.hpp>
#include <Graphics/Sprite/Sprite.hpp>
#include <Graphics/Primitives/Cube/Cube.hpp>
#include <Graphics/Primitives/Grid/Grid.hpp>

#include <Camera/Camera.hpp>

#include <memory>
#include <array>

namespace Editor 
{

    class BaseLayer 
        : public Engine::ILayer
    {
    public:
        BaseLayer();
        virtual ~BaseLayer() = default;

        bool init() override;
        void update(float deltaTime)override;
        void render()override;

        void SetCamera(smpl::Camera& camera) { m_camera = &camera; }

    private:
        void initShaders();
        void initTextures();
        void initObjects();
        void renderCubes();

    private:
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

        smpl::Camera* m_camera;
        bool m_initialized;
    };

}
