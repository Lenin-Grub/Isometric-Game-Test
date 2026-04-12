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
        // Sprite shader
        m_vertex_shader3.loadFromFile("shaders/primitive_texture_shader.vert", smpl::Shader::Type::Vertex);
        m_fragment_shader3.loadFromFile("shaders/primitive_texture_shader.frag", smpl::Shader::Type::Fragment);
        m_shader_plane.create(m_vertex_shader3, m_fragment_shader3);
    }

    void BaseLayer::initTextures() 
    {
        m_texture_horde.loadTextureFromFile("res/horde.png");
        m_texture_rus.loadTextureFromFile("res/parchment.png");
        m_texture_stonewall.loadTextureFromFile("res/stone_wall.png");
    }

    void BaseLayer::initObjects() 
    {
        m_sprite1.setShader(m_shader_plane);
        m_sprite2.setShader(m_shader_plane);
        m_sprite3.setShader(m_shader_plane);

        m_grid = std::make_unique<smpl::Grid>();
        m_grid->init();
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
