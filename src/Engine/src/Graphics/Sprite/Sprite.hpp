#pragma once
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <array>

#include <Graphics/Texture/Texture.hpp>
#include <Graphics/Shader/Shader.hpp>
#include <Camera/Camera.hpp>

namespace smpl 
{
    class Sprite
    {
    public:
        Sprite();

        ~Sprite();

        void setShader(smpl::ShaderProgram& shader);

    public:

        void draw(const Texture2D& texture, smpl::Camera camera);

        void setPosition(const glm::vec3& pos)
        {
            m_position = pos;
        }

        void setRotation(const glm::vec3& angle)
        {
            m_rotation = angle;
        }

        void setScale(const glm::vec3& scale)
        {
            m_scale = scale;
        }

        void setColor(const glm::vec3& color)
        {
            m_color = color;
        }

        glm::vec3 getPosition() const { return m_position; }
        glm::vec3 getRotation() const { return m_rotation; }
        glm::vec3 getScale()    const { return m_scale; }
        glm::vec3 getColor()    const { return m_color; }

    private:
        void initRenderData();

    private:
        smpl::ShaderProgram* m_shader = nullptr;

        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;
        glm::vec3 m_color;

        GLuint vao = 0;
        GLuint vbo = 0;
        GLuint ibo = 0;

        std::array<GLuint, 6> m_indices;
    };
}