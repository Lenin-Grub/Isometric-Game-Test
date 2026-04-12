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

    private:
        // Rendering resources
        smpl::Shader m_vertex_shader;
        smpl::Shader m_fragment_shader;
        smpl::ShaderProgram m_shader_cube;

        smpl::Shader m_vertex_shader3;
        smpl::Shader m_fragment_shader3;
        smpl::ShaderProgram m_shader_plane;

        smpl::Sprite m_sprite1;
        smpl::Sprite m_sprite2;
        smpl::Sprite m_sprite3;

        std::unique_ptr<smpl::Grid> m_grid;

        smpl::Texture2D m_texture_horde;
        smpl::Texture2D m_texture_rus;
        smpl::Texture2D m_texture_stonewall;

        smpl::Camera* m_camera;
        bool m_initialized;
    };

}
