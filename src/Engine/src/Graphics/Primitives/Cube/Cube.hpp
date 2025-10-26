#pragma once
#include <glad/glad.h>
#include <glm/ext/vector_float3.hpp>

namespace smpl
{

    class Cube
    {
    public:
        Cube();
        ~Cube();

        const std::array<float, 120>&   getVertices() const;
        const std::array<GLuint, 36>&   getIndices()  const;

        void setPosition(glm::vec3 position);
        const glm::vec3& getPosition() const;


    private:
        glm::vec3 m_position{0.0f};

        std::array<float, 120> m_vertices = {
            // Front face
            -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
            -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,

            // Back face
            -1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
             1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
             1.0f,  1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f,  1.0f,  1.0f,  1.0f, 1.0f,

            // Right face
             1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
             1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
             1.0f,  1.0f,  1.0f,  0.0f, 1.0f,

             // Left face
             -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
             -1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
             -1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
             -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,

             // Top face
             -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
              1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
              1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
             -1.0f,  1.0f,  1.0f,  0.0f, 0.0f,

             // Bottom face
             -1.0f, -1.0f,  1.0f,  0.0f, 1.0f,
              1.0f, -1.0f,  1.0f,  1.0f, 1.0f,
              1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
             -1.0f, -1.0f, -1.0f,  0.0f, 0.0f
        };

        std::array<GLuint, 36> m_indices = {
              0,  1,  2,  0,  2,  3,  // front
              4,  5,  6,  4,  6,  7,  // back
              8,  9, 10,  8, 10, 11,  // right
              12, 13, 14, 12, 14, 15, // left
              16, 17, 18, 16, 18, 19, // top
              20, 21, 22, 20, 22, 23  // bottom
        };
    };
}