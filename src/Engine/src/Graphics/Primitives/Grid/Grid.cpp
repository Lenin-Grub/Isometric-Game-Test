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
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
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

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    bool Grid::initRenderData()
    {
        bool result = false;
        // (x, y, z), UV (u, v)
        float vertices[] = { -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                              1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                             -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                              1.0f,  1.0f, 0.0f, 1.0f, 1.0f };

        // VAO
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // VBO
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // IBO
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices.data(), GL_STATIC_DRAW);

        // BufferLayout
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Off VAO
        glBindVertexArray(0);
        return result = true;
    }
}