#pragma once

#include <glad/glad.h>

#include <array>
#include <Graphics/Shader/Shader.hpp>
#include <Camera/Camera.hpp>

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

        GLuint vao = 0;
        GLuint vbo = 0;
        GLuint ibo = 0;

        std::array<GLuint, 6> m_indices = { 0, 1, 2, 1, 3, 2 };
    };
}