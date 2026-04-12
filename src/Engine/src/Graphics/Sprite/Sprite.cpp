#include "Sprite.hpp"
#include <Camera/Camera.hpp>

namespace smpl 
{
    Sprite::Sprite()
        : m_position(0.0f)
        , m_rotation(0.0f)
        , m_scale(1.0f)
        , m_color(1.0f)
    {
    }

    Sprite::~Sprite()
    {
    }

    void Sprite::draw(const Texture2D& texture, smpl::Camera& camera)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::translate(model, m_position);
        model           = glm::rotate(model, glm::radians(m_rotation.x), glm::vec3(1, 0, 0));
        model           = glm::rotate(model, glm::radians(m_rotation.y), glm::vec3(0, 1, 0));
        model           = glm::rotate(model, glm::radians(m_rotation.z), glm::vec3(0, 0, 1));
        model           = glm::scale(model, glm::vec3(m_scale));
        model           = glm::translate(model, glm::vec3(-0.5f * m_scale.x, -0.5f * m_scale.y, 0.0f));
        model           = glm::translate(model, glm::vec3(0.5f * m_scale.x, 0.5f * m_scale.y, 0.0f));

        m_shader.use();
        m_shader.setUniformMatrix("model", model);
        m_shader.setUniform1i("texture1", 0);
        m_shader.setUniform4f("sprite_color", m_color);
        m_shader.setUniformMatrix("projection", camera.getProjectionMatrix());
        m_shader.setUniformMatrix("view", camera.getViewMatrix());

        glActiveTexture(GL_TEXTURE0);
        texture.bind();

        m_VAO->bind();
        m_IBO->bind();

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_data.size()), GL_UNSIGNED_INT, nullptr);

        m_IBO->unbind();
        m_VAO->unbind();
    }

    bool Sprite::init()
    {
        if (!m_vertex_shader.loadFromFile("shaders/primitive_texture_shader.vert", smpl::Shader::Type::Vertex))
            return false;
        if (!m_fragment_shader.loadFromFile("shaders/primitive_texture_shader.frag", smpl::Shader::Type::Fragment))
            return false;
        if (!m_shader.create(m_vertex_shader, m_fragment_shader))
            return false;
        if (!initRenderData())
            return false;
        return true;
    }

    bool Sprite::initRenderData()
    {
        m_VAO.reset();
        m_VBO.reset();
        m_IBO.reset();

        m_VAO = std::make_unique<VertexArray>();
        m_VBO = std::make_unique<VertexBuffer>();
        m_IBO = std::make_unique<IndexBuffer>();

        // (x, y, z), UV (u, v)
        float vertices[] = { -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                              1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                             -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                              1.0f,  1.0f, 0.0f, 1.0f, 1.0f };

        m_VAO->bind();
        m_VBO->create(vertices, sizeof(vertices));
        m_IBO->create(indices_data.data(), static_cast<unsigned int>(indices_data.size()));

        smpl::BufferLayout layout
        {
            smpl::ShaderDataType::Float3, // position
            smpl::ShaderDataType::Float2  // texture2D
        };
        layout.setLayout();

        return true;
    }
}