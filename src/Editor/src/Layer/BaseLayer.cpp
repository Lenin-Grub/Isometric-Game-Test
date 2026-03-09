#include "BaseLayer.hpp"
#include <Log/Log.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Editor 
{

    BaseLayer::BaseLayer()
        : m_camera(nullptr)
        , m_initialized(false)
    {
        LOG_INFO("BaseLayer created");
    }

    void BaseLayer::initShaders() 
    {
        // Cube shader with mixed 2 texture
        m_vertex_shader.loadFromFile("shaders/primitive_texture_shader.vert", smpl::Shader::Type::Vertex);
        m_fragment_shader.loadFromFile("shaders/primitive_texture_shader.frag", smpl::Shader::Type::Fragment);
        m_shader_cube.create(m_vertex_shader, m_fragment_shader);

        // Sprite shader
        m_vertex_shader3.loadFromFile("shaders/primitive_texture_shader.vert", smpl::Shader::Type::Vertex);
        m_fragment_shader3.loadFromFile("shaders/primitive_texture_shader.frag", smpl::Shader::Type::Fragment);
        m_shader_plane.create(m_vertex_shader3, m_fragment_shader3);
    }

    void BaseLayer::initTextures() 
    {
        m_texture_horde.loadTextureFromFile("res/horde.png");
        m_texture_rus.loadTextureFromFile("res/rus.png");
        m_texture_stonewall.loadTextureFromFile("res/stone_wall.png");
    }

    void BaseLayer::initObjects() 
    {
        m_sprite1.setShader(m_shader_plane);
        m_sprite2.setShader(m_shader_plane);
        m_sprite3.setShader(m_shader_plane);

        m_grid = std::make_unique<smpl::Grid>();
        m_grid->init();

        m_positions = {
            glm::vec3(-2.f, -2.f, -4.f),
            glm::vec3(-5.f,  0.f,  3.f),
            glm::vec3( 2.f,  1.f, -2.f),
            glm::vec3( 4.f, -3.f,  3.f),
            glm::vec3( 1.f, -7.f,  1.f),
            glm::vec3( 1.f,  1.f,  1.f)
        };

        smpl::BufferLayout layout
        {
            smpl::ShaderDataType::Float3, // position
            smpl::ShaderDataType::Float2  // texture2D
        };

        m_vao = std::make_unique<smpl::VertexArray>();
        m_vbo = std::make_unique<smpl::VertexBuffer>(m_cube.getVertices().data(), sizeof(m_cube.getVertices()), layout);
        m_index_buffer = std::make_unique<smpl::IndexBuffer>(m_cube.getIndices().data(), static_cast<uint32_t>(m_cube.getIndices().size()));

        m_vao->addVertexBuffer(*m_vbo);
        m_vao->setIndexBuffer(*m_index_buffer);
    }

    void BaseLayer::renderCubes() 
    {
        m_vao->bind();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture_stonewall.getID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_texture_horde.getID());

        m_shader_cube.use();
        m_shader_cube.setUniform1i("texture1", 0);
        m_shader_cube.setUniform1i("texture2", 1);
        m_shader_cube.setUniformMatrix("view", m_camera->getViewMatrix());
        m_shader_cube.setUniformMatrix("projection", m_camera->getProjectionMatrix());
        m_shader_cube.setUniform4f("sprite_color", glm::vec4(1.0f));

        glm::mat4 base_transform = glm::mat4(1.0f);
        glm::mat4 rotation       = glm::mat4(1.0f);

        rotation = glm::rotate(rotation, glm::radians(0.f), glm::vec3(1, 0, 0)); // Roll
        rotation = glm::rotate(rotation, glm::radians(0.f), glm::vec3(0, 1, 0)); // Pitch
        rotation = glm::rotate(rotation, glm::radians(0.f), glm::vec3(0, 0, 1)); // Yaw

        base_transform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        base_transform = base_transform * rotation;
        base_transform = glm::scale(base_transform, glm::vec3(1.f, 1.f, 1.f));

        for (const glm::vec3& pos : m_positions) 
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), pos) * base_transform;
            m_shader_cube.setUniformMatrix("model", model);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_vao->getIndexesCount()), GL_UNSIGNED_INT, nullptr);
        }
        m_vao->unbind();
    }

    bool BaseLayer::init()
    {
        if (!m_initialized) 
        {
            initShaders();
            initTextures();
            initObjects();
            m_initialized = true;
            return true;
        }
        return false;
    }

    void BaseLayer::update(float deltaTime)
    {
        if (!isActive() || !isVisible() || !m_initialized || !m_camera)
            return;

    }

    void BaseLayer::render() 
    {
        if (!isVisible() || !m_initialized || !m_camera)
            return;

         //Grid
        m_grid->draw(*m_camera);

        // Cubes
        renderCubes();

        // Sprites
        m_sprite1.setPosition(glm::vec3(0, 0, 7));
        m_sprite1.setScale(glm::vec3(7, 4, 1));
        m_sprite3.setPosition(glm::vec3(-15, -15, 7));
        m_sprite3.setScale(glm::vec3(10, 10, 1));
        m_sprite3.draw(m_texture_horde, *m_camera);
        m_sprite1.draw(m_texture_rus, *m_camera);
        m_sprite2.draw(m_texture_rus, *m_camera);
    }
}
