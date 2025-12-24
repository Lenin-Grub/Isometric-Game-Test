#include "Cube.hpp"

namespace smpl 
{
    Cube::Cube()
    {
    }

    Cube::~Cube()
    {
    }

    const std::array<float, 120>& Cube::getVertices() const
    {
        return m_vertices;
    }

    const std::array<GLuint, 36>& Cube::getIndices() const
    {
        return m_indices;
    }
    void Cube::setPosition(glm::vec3 position)
    {
        m_position = position;
    }
    const glm::vec3& Cube::getPosition() const
    {
        return m_position;
    }
}