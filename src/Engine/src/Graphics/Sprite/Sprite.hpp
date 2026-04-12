#pragma once
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <array>

#include <Graphics/Texture/Texture.hpp>
#include <Graphics/Shader/Shader.hpp>
#include <Camera/Camera.hpp>
#include <Log/Log.hpp>
#include <Graphics/VertexArray/VertexArray.hpp>
#include <Graphics/IndexBuffer/IndexBuffer.hpp>
#include <Graphics/VertexBuffer/VertexBuffer.hpp>

namespace smpl 
{
    class Sprite
    {
    public:
        Sprite();

        ~Sprite();


    public:

        void draw(const Texture2D& texture, smpl::Camera& camera);

        bool init();

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

        void setColor(const glm::vec4& color)
        {
            m_color = color;
        }

        glm::vec3 getPosition() const { return m_position; }
        glm::vec3 getRotation() const { return m_rotation; }
        glm::vec3 getScale()    const { return m_scale; }
        glm::vec3 getColor()    const { return m_color; }

    private:
        bool initRenderData();

    private:

        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;
        glm::vec4 m_color;

        smpl::ShaderProgram m_shader;
        smpl::Shader m_vertex_shader;
        smpl::Shader m_fragment_shader;

        std::unique_ptr<VertexArray> m_VAO = nullptr;
        std::unique_ptr<VertexBuffer> m_VBO = nullptr;
        std::unique_ptr<IndexBuffer> m_IBO = nullptr;

        std::vector<unsigned int> indices_data = { 0, 1, 2, 1, 3, 2 };
    };
}