#include "Sprite.hpp"
#include <Camera/Camera.hpp>

namespace smpl 
{
    Sprite::Sprite()
        : m_shader(nullptr)
        , m_position(0.0f)
        , m_rotation(0.0f)
        , m_scale(1.0f)
        , m_color(1.0f)
    {
        //initRenderData();
    }

    Sprite::~Sprite()
    {
        if (vao != 0) glDeleteVertexArrays(1, &vao);
        if (vbo != 0) glDeleteBuffers(1, &vbo);
        if (ibo != 0) glDeleteBuffers(1, &ibo);
    }

    void Sprite::setShader(smpl::ShaderProgram& shader)
    {
        m_shader = &shader;
        initRenderData();
    }

    void Sprite::draw(const Texture2D& texture, glm::vec2 size, smpl::Camera camera)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, m_position);
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
        model = glm::scale    (model, glm::vec3(m_scale));
        model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));

        m_shader->use();
        m_shader->setUniformMatrix("model", model);
        m_shader->setUniform1i("texture1", 0);
        m_shader->setUniformMatrix("projection", camera.getProjectionMatrix());
        m_shader->setUniformMatrix("view", camera.getViewMatrix());

        // 1st texture
        glActiveTexture(GL_TEXTURE0);
        texture.bind();

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    void Sprite::initRenderData()
    {
        float vertices[] = {
            //   X     Y     Z     U     V
            -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
             1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
             1.0f,  1.0f, 0.0f,  1.0f, 1.0f };

        m_indices = { 0, 2, 1, 1, 2, 3 };

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // XYZ
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // UV
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }
}