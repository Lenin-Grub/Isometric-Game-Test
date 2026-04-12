#include "Grid.hpp"

#include <glm/ext/matrix_transform.hpp>

#include <array>

namespace smpl
{
    Grid::Grid()
    {
    }

    Grid::~Grid()
    {
    }

    bool Grid::init()
    {
        if (!m_vertex_shader.loadFromFile("shaders/grid_shader.vert", smpl::Shader::Type::Vertex))
            return false;
        if (!m_fragment_shader.loadFromFile("shaders/grid_shader.frag", smpl::Shader::Type::Fragment))
            return false;
        if (!m_shader.create(m_vertex_shader, m_fragment_shader))
            return false;
        if (!initRenderData())
            return false;
        return true;
    }

    void Grid::draw(smpl::Camera camera)
    {
        glm::mat4 grid_transform = glm::mat4(1.0f);
        grid_transform           = glm::rotate(grid_transform, glm::radians(0.0f), glm::vec3(1, 0, 0));
        grid_transform           = glm::scale(grid_transform, glm::vec3(50.0f, 50.0f, 1.0f));
        glm::mat4 gridMVP        = camera.getProjectionMatrix() * camera.getViewMatrix() * grid_transform;

        m_shader.use();
        m_shader.setUniformMatrix("grid_mvp", gridMVP);
        m_shader.setUniform1f("grid_step", 0.025f);
        m_shader.setUniform3f("grid_color", 0.6f, 0.6f, 0.6f);

        m_VAO.bind();
        m_IBO.bind();

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);

        m_IBO.unbind();
        m_VAO.unbind();
    }

    bool Grid::initRenderData()
    {
        bool result = false;
        // (x, y, z), UV (u, v)
        float vertices[] = { -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                              1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                             -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                              1.0f,  1.0f, 0.0f, 1.0f, 1.0f };

        m_VAO.bind();
        m_VBO.create(vertices, sizeof(vertices));
        m_IBO.create(m_indices.data(), static_cast<unsigned int>(m_indices.size()));

        smpl::BufferLayout layout
        {
            smpl::ShaderDataType::Float3, // position
            smpl::ShaderDataType::Float2  // texture2D
        };
        layout.setLayout();

        m_VAO.unbind();

        return result = true;
    }
}