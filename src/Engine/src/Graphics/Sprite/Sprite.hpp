#pragma once

#include <Graphics/VertexArray/VertexArray.hpp>
#include <Graphics/VertexBuffer/VertexBuffer.hpp>
#include <Graphics/Shader/Shader.hpp>
#include <Graphics/Texture/Texture.hpp>

#include <glm/glm.hpp>
#include <memory>

namespace smpl
{
    class Sprite
    {
    public:
        struct Rect
        {
            float left, top, width, height;
        };

        Sprite();
        ~Sprite();

        void create(const Texture& texture, const Rect& rect);
        void render(const Texture& texture, const ShaderProgram& shader) const;

        void setPosition(const glm::vec3& position);
        void setRotation(const glm::vec3& rotation);
        void setScale   (const glm::vec3& scale);

        glm::vec3 getPosition() const { return m_position; }
        glm::vec3 getRotation() const { return m_rotation; }
        glm::vec3 getScale()    const { return m_scale;    }

    private:
        std::unique_ptr<smpl::VertexArray>  m_vao;
        std::unique_ptr<smpl::VertexBuffer> m_vbo;
        std::unique_ptr<smpl::IndexBuffer>  m_ebo;

        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;
    };
}