#include <Graphics/Sprite/Sprite.hpp>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace smpl
{

    Sprite::Sprite()
        : m_position(0.0f, 0.0f, 0.0f)
        , m_rotation(0.0f, 0.0f, 0.0f)
        , m_scale(1.0f, 1.0f, 1.0f)
    {
    }

    Sprite::~Sprite() = default;

    void Sprite::create(const Texture& texture, const Rect& rect)
    {
        float tex_width = static_cast<float>(texture.getWidth());
        float tex_height = static_cast<float>(texture.getHeight());

        float u1 = rect.left / tex_width;
        float v1 = rect.top / tex_height;
        float u2 = (rect.left + rect.width) / tex_width;
        float v2 = (rect.top + rect.height) / tex_height;

        // Pos + UV
        float vertices[] = {
            // x    y      z      u   v
             0.0f,  0.0f,  0.0f,  u1, v2,
             1.0f,  0.0f,  0.0f,  u2, v2,
             1.0f, -1.0f,  0.0f,  u2, v1,
             0.0f, -1.0f,  0.0f,  u1, v1
        };

        uint32_t indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        BufferLayout layout{
            ShaderDataType::Float3, // Position
            ShaderDataType::Float2  // UV
        };

        m_vao = std::make_unique<VertexArray>();
        m_vbo = std::make_unique<VertexBuffer>(vertices, sizeof(vertices), layout);
        m_ebo = std::make_unique<IndexBuffer>(indices, 6);

        m_vao->addVertexBuffer(*m_vbo);
        m_vao->setIndexBuffer(*m_ebo);


        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindTexture(GL_TEXTURE_2D, texture.getTextureID());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    void Sprite::render(const Texture& texture, const ShaderProgram& shader) const
    {
        if (!m_vao || !texture.getTextureID()) return;

        m_vao->bind();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.getTextureID());

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, m_position);
        model = glm::rotate(model, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale (model, m_scale);

        shader.setUniformMatrix("model", model);
        shader.setUniform1i("texture1", 0);

        glDrawElements(GL_TRIANGLES, m_ebo->getCount(), GL_UNSIGNED_INT, 0);

        m_vao->unbind();
    }

    void Sprite::setPosition(const glm::vec3& position)
    {
        m_position = position;
    }

    void Sprite::setRotation(const glm::vec3& rotation)
    {
        m_rotation = rotation;
    }

    void Sprite::setScale(const glm::vec3& scale)
    {
        m_scale = scale;
    }
}