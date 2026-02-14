#include "Application.hpp"

namespace EDITOR
{
    Application::Application()
    {
    }

    void Application::initShaders() 
    {
        // Cube shader with mixed 2 texture
        m_vertex_shader.loadFromFile("shaders/primitive_texture_shader.vert", smpl::Shader::Type::Vertex);
        m_fragment_shader.loadFromFile("shaders/primitive_texture_shader.frag", smpl::Shader::Type::Fragment);
        m_shader_cube.create(m_vertex_shader, m_fragment_shader);

        // Grid shader
        m_vertex_shader2.loadFromFile("shaders/grid_shader.vert", smpl::Shader::Type::Vertex);
        m_fragment_shader2.loadFromFile("shaders/grid_shader.frag", smpl::Shader::Type::Fragment);
        m_shader_grid.create(m_vertex_shader2, m_fragment_shader2);

        // Sprite shader
        m_vertex_shader3.loadFromFile("shaders/plane_1texture_shader.vert", smpl::Shader::Type::Vertex);
        m_fragment_shader3.loadFromFile("shaders/plane_1texture_shader.frag", smpl::Shader::Type::Fragment);
        m_shader_plane.create(m_vertex_shader3, m_fragment_shader3);
    }

    void Application::initTextures() 
    {
        m_state.texture1.loadFromFile("res/stone_wall.png");
        m_state.texture2.loadFromFile("res/horde.png");
        m_first_texture2d.loadTextureFromFile("res/ussr.png");
        m_second_texture2d.loadTextureFromFile("res/rus.png");
    }

    void Application::initObjects() 
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

    void Application::setupEventCallbacks() 
    {
        m_window->EventCallback = [this](Core::Event& event) 
        {
            LOG_INFO("{}", event.toString());

            Core::EventDispatcher dispatcher(event);

            dispatcher.dispatch<Core::KeyPressedEvent>([this](Core::KeyPressedEvent& e) 
            {
                int keycode = e.getKeyCode();

                if (keycode == smpl::Key::Code::Escape) 
                {
                    m_window->close();
                    return true;
                }

                if (m_state.camera.getProjectionMode() == smpl::Camera::Projection::Perspective) 
                {
                    float camera_speed = 30.0f * m_state.delta_time;

                    if (keycode == smpl::Key::Code::W)
                        m_state.camera.moveForward(camera_speed);
                    else if (keycode == smpl::Key::Code::S)
                        m_state.camera.moveForward(-camera_speed);
                    else if (keycode == smpl::Key::Code::A)
                        m_state.camera.moveRight(-camera_speed);
                    else if (keycode == smpl::Key::Code::D)
                        m_state.camera.moveRight(camera_speed);
                    else if (keycode == smpl::Key::Code::Space)
                        m_state.camera.moveUp(camera_speed);
                    else if (keycode == smpl::Key::Code::LCtrl)
                        m_state.camera.moveUp(-camera_speed);

                    float rot_speed = 50.0f * m_state.delta_time;
                    glm::vec3 rot_delta(0);

                    if (keycode == smpl::Key::Code::Up)
                        rot_delta.x += rot_speed;
                    else if (keycode == smpl::Key::Code::Down)
                        rot_delta.x -= rot_speed;
                    else if (keycode == smpl::Key::Code::Left)
                        rot_delta.y += rot_speed;
                    else if (keycode == smpl::Key::Code::Right)
                        rot_delta.y -= rot_speed;

                    if (rot_delta != glm::vec3(0))
                        m_state.camera.setRotation(m_state.camera.getRotation() + rot_delta);
                }

                if (m_state.camera.getProjectionMode() == smpl::Camera::Projection::Isometric) 
                {
                    float camera_speed = 30.0f * m_state.delta_time;
                    glm::vec3 pos = m_state.camera.getPosition();

                    if (keycode == smpl::Key::Code::W) 
                    {
                        pos.x -= camera_speed;
                        pos.y += camera_speed;
                    }
                    else if (keycode == smpl::Key::Code::S) 
                    {
                        pos.x += camera_speed;
                        pos.y -= camera_speed;
                    }
                    else if (keycode == smpl::Key::Code::A) 
                    {
                        pos.x -= camera_speed;
                        pos.y -= camera_speed;
                    }
                    else if (keycode == smpl::Key::Code::D)
                    {
                        pos.x += camera_speed;
                        pos.y += camera_speed;
                    }
                    else if (keycode == smpl::Key::Code::Space)
                        pos.z += camera_speed;
                    else if (keycode == smpl::Key::Code::LCtrl)
                        pos.z -= camera_speed;

                    m_state.camera.setPosition(pos);
                }

                return true;
            });

            static bool first_mouse = true;
            static double last_mouse_x = 0.0;
            static double last_mouse_y = 0.0;

            dispatcher.dispatch<Core::MouseMovedEvent>([this](Core::MouseMovedEvent& e) 
            {
                if (glfwGetInputMode(&m_window->getWindow(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED) 
                {
                    double current_mouse_x = e.getX();
                    double current_mouse_y = e.getY();

                    if (first_mouse) 
                    {
                        last_mouse_x = current_mouse_x;
                        last_mouse_y = current_mouse_y;
                        first_mouse = false;
                    }

                    float mouse_sensitivity = 0.1f;
                    float delta_x = (current_mouse_x - last_mouse_x) * mouse_sensitivity;
                    float delta_y = (last_mouse_y - current_mouse_y) * mouse_sensitivity;
                    last_mouse_x = current_mouse_x;
                    last_mouse_y = current_mouse_y;

                    if (m_state.camera.getProjectionMode() == smpl::Camera::Projection::Perspective) 
                    {
                        glm::vec3 current_rot = m_state.camera.getRotation();
                        current_rot.y -= delta_x;
                        current_rot.x += delta_y;

                        current_rot.x = glm::clamp(current_rot.x, -89.0f, 89.0f);

                        m_state.camera.setRotation(current_rot);
                    }
                }
                return true;
            });

            dispatcher.dispatch<Core::MouseButtonPressedEvent>([this](Core::MouseButtonPressedEvent& e) 
            {
                if (e.getMouseButton() == static_cast<int>(smpl::Mouse::Button::Right)) 
                {
                    if (glfwGetInputMode(&m_window->getWindow(), GLFW_CURSOR) != GLFW_CURSOR_DISABLED) 
                    {
                        glfwSetInputMode(&m_window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                    }
                }
                return true;
            });

            dispatcher.dispatch<Core::MouseButtonReleasedEvent>([this](Core::MouseButtonReleasedEvent& e) 
            {
                if (e.getMouseButton() == static_cast<int>(smpl::Mouse::Button::Right)) 
                {
                    if (glfwGetInputMode(&m_window->getWindow(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED) 
                    {
                        glfwSetInputMode(&m_window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                        first_mouse = true;
                    }
                }
                return true;
            });

            dispatcher.dispatch<Core::MouseScrolledEvent>([this](Core::MouseScrolledEvent& e) 
            {
                float scroll_y = static_cast<float>(e.getYOffset());

                if (m_state.camera.getProjectionMode() == smpl::Camera::Projection::Perspective) 
                {
                    float rot_speed = 50.0f * m_state.delta_time;
                    m_state.camera.moveUp(scroll_y > 0 ? rot_speed : -rot_speed);
                }
                else if (m_state.camera.getProjectionMode() == smpl::Camera::Projection::Isometric) 
                {
                    float scroll_sensitivity = 1.0f;
                    float new_zoom = m_state.zoom - scroll_y * scroll_sensitivity;
                    m_state.zoom = glm::clamp(new_zoom, 1.0f, 100.0f);
                    m_state.camera.setZoom(m_state.zoom);
                }
                return true;
            });

            return false;
        };
    }

    void Application::render()
    {
        m_window->clear(smpl::Color(50, 50, 50));

        // Cubes
        renderCubes();

        // Grid
        m_grid->draw(m_state.camera);

        // Sprites
        m_sprite1.setPosition(glm::vec3(0, 0, 7));
        m_sprite1.setScale(glm::vec3(7, 4, 1));
        m_sprite1.draw(m_second_texture2d, m_state.camera);
        m_sprite2.draw(m_second_texture2d, m_state.camera);
        m_sprite3.setPosition(glm::vec3(-15, -15, 7));
        m_sprite3.setScale(glm::vec3(10, 10, 1));
        m_sprite3.draw(m_first_texture2d, m_state.camera);

        m_window->display();
    }

    void Application::renderCubes() 
    {
        m_vao->bind();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_state.texture1.getTextureID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_state.texture2.getTextureID());

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

    bool Application::init()
    {
        m_settings.loadFromFile("config/editor_settings.json");
        m_window = std::make_unique<smpl::Window>(m_settings);
        m_window->create();

        smpl::Gui::initImGui(*m_window);
        smpl::Gui::initImGuiFont();

        initShaders();
        initTextures();
        initObjects();
        setupEventCallbacks();

        return true;
    }

    void Application::run()
    {
        while (m_window->isOpen()) 
        {
            float current_frame = static_cast<float>(glfwGetTime());
            m_state.delta_time = current_frame - m_state.last_frame;
            m_state.last_frame = current_frame;

            glfwPollEvents();
            render();
        }
    }
}