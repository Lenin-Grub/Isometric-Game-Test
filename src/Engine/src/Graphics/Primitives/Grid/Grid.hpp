#pragma once

#include <glad/glad.h>

#include <array>
#include <Graphics/Shader/Shader.hpp>
#include <Camera/Camera.hpp>
#include <Graphics/IndexBuffer/IndexBuffer.hpp>
#include <Graphics/VertexArray/VertexArray.hpp>
#include <Graphics/VertexBuffer/VertexBuffer.hpp>

namespace smpl
{
    class Grid
    {
    public:
        Grid();
        ~Grid();

    public:

        bool init();
        void draw(smpl::Camera camera);

    private:
        bool initRenderData();

    private:
        smpl::ShaderProgram m_shader;
        smpl::Shader m_vertex_shader;
        smpl::Shader m_fragment_shader;

        VertexArray  m_VAO;
        VertexBuffer m_VBO;
        IndexBuffer  m_IBO;

        std::array<GLuint, 6> m_indices = { 0, 1, 2, 1, 3, 2 };
    };
}