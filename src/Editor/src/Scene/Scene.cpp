#include "Scene.hpp"
#include <Log/Log.hpp>

namespace Editor
{
    Scene::Scene(const std::string& name)
        : m_scene_name(name)
        , m_is_active(false)
        , m_is_loaded(false)
    {
        LOG_INFO("Scene '{}' created", m_scene_name);
    }

    Scene::~Scene()
    {
        if (m_is_loaded)
        {
            unload();
        }
        LOG_INFO("Scene '{}' destroyed", m_scene_name);
    }

    void Scene::initShaders()
    {
        // Grid shader
        m_vertex_shader2.loadFromFile("shaders/grid_shader.vert", smpl::Shader::Type::Vertex);
        m_fragment_shader2.loadFromFile("shaders/grid_shader.frag", smpl::Shader::Type::Fragment);
        m_shader_grid.create(m_vertex_shader2, m_fragment_shader2);

        // Cube shader with mixed 2 texture
        m_vertex_shader.loadFromFile("shaders/primitive_texture_shader.vert", smpl::Shader::Type::Vertex);
        m_fragment_shader.loadFromFile("shaders/primitive_texture_shader.frag", smpl::Shader::Type::Fragment);
        m_shader_cube.create(m_vertex_shader, m_fragment_shader);

        // Sprite shader
        m_vertex_shader3.loadFromFile("shaders/plane_1texture_shader.vert", smpl::Shader::Type::Vertex);
        m_fragment_shader3.loadFromFile("shaders/plane_1texture_shader.frag", smpl::Shader::Type::Fragment);
        m_shader_plane.create(m_vertex_shader3, m_fragment_shader3);
    }

    void Scene::initTextures()
    {
        m_texture1.loadFromFile("res/stone_wall.png");
        m_texture2.loadFromFile("res/horde.png");
        m_first_texture2d.loadTextureFromFile("res/ussr.png");
        m_second_texture2d.loadTextureFromFile("res/rus.png");
    }

    void Scene::initObjects()
    {
        m_sprite1.setShader(m_shader_plane);
        m_sprite2.setShader(m_shader_plane);
        m_sprite3.setShader(m_shader_plane);

        m_grid = std::make_unique<smpl::Grid>(m_shader_grid);

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

    bool Scene::init()
    {
        if (m_is_loaded)
        {
            LOG_WARN("Scene '{}' is already loaded", m_scene_name);
            return false;
        }

        LOG_INFO("Initializing scene '{}'", m_scene_name);

        initShaders();
        initTextures();
        initObjects();

        m_is_loaded = true;
        return true;
    }

    void Scene::update()
    {
        if (!m_is_active || !m_is_loaded)
            return;

        // Update scene logic here
    }

    void Scene::renderCubes()
    {
        m_vao->bind();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture1.getTextureID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_texture2.getTextureID());

        m_shader_cube.use();
        m_shader_cube.setUniform1i("texture1", 0);
        m_shader_cube.setUniform1i("texture2", 1);
        m_shader_cube.setUniformMatrix("view", m_state.camera.getViewMatrix());
        m_shader_cube.setUniformMatrix("projection", m_state.camera.getProjectionMatrix());

        glm::mat4 base_transform = glm::mat4(1.0f);
        glm::mat4 rotation       = glm::mat4(1.0f);

        rotation = glm::rotate(rotation, glm::radians(m_state.rotate.x), glm::vec3(1, 0, 0)); // Roll
        rotation = glm::rotate(rotation, glm::radians(m_state.rotate.y), glm::vec3(0, 1, 0)); // Pitch
        rotation = glm::rotate(rotation, glm::radians(m_state.rotate.z), glm::vec3(0, 0, 1)); // Yaw

        base_transform = glm::translate(glm::mat4(1.0f), m_state.translate);
        base_transform = base_transform * rotation;
        base_transform = glm::scale(base_transform, m_state.scale);

        for (const glm::vec3& pos : m_positions)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), pos) * base_transform;
            m_shader_cube.setUniformMatrix("model", model);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_vao->getIndexesCount()), GL_UNSIGNED_INT, nullptr);
        }
        m_vao->unbind();
    }

    void Scene::render()
    {
        if (!m_is_active || !m_is_loaded)
            return;
        // Grid
        m_grid->draw(m_state.camera);
        
        // Cubes
        renderCubes();

        // Sprites
        m_sprite1.setPosition(glm::vec3(0, 0, 7));
        m_sprite1.setScale(glm::vec3(7, 4, 1));
        m_sprite3.setPosition(glm::vec3(-15, -15, 7));
        m_sprite3.setScale(glm::vec3(10, 10, 1));
        m_sprite3.draw(m_first_texture2d, m_state.camera);
        m_sprite1.draw(m_second_texture2d, m_state.camera);
        m_sprite2.draw(m_second_texture2d, m_state.camera);
    }

    bool Scene::load()
    {
        if (m_is_loaded)
        {
            LOG_WARN("Scene '{}' is already loaded", m_scene_name);
            return false;
        }

        LOG_INFO("Loading scene '{}'", m_scene_name);

        if (!init())
        {
            LOG_ERROR("Failed to initialize scene '{}'", m_scene_name);
            return false;
        }

        m_is_loaded = true;
        return true;
    }

    bool Scene::unload()
    {
        if (!m_is_loaded)
        {
            LOG_WARN("Scene '{}' is not loaded", m_scene_name);
            return false;
        }

        LOG_INFO("Unloading scene '{}'", m_scene_name);

        // Clean up resources here

        m_is_loaded = false;
        m_is_active = false;
        return true;
    }

    bool Scene::isActive()
    {
        return m_is_active;
    }

    void Scene::activate()
    {
        if (!m_is_loaded)
        {
            LOG_WARN("Cannot activate scene '{}' - it's not loaded", m_scene_name);
            return;
        }

        m_is_active = true;
        LOG_INFO("Scene '{}' activated", m_scene_name);
    }
}