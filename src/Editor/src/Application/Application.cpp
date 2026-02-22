#include "Application.hpp"

namespace Editor
{
    Application::Application()
        : m_main_scene("MainScene")
        , m_displays(m_main_scene)
    {
    }

    void Application::setupEventCallbacks()
    {
        m_window->EventCallback = [this](Core::Event& event)
        {
            //LOG_INFO("{}", event.toString());

            Core::EventDispatcher dispatcher(event);

            dispatcher.dispatch<Core::KeyPressedEvent>([this](Core::KeyPressedEvent& e)
                    {
                        int keycode = e.getKeyCode();

                        if (keycode == smpl::Key::Code::Escape)
                        {
                            m_window->close();
                            return true;
                        }

                        if (m_main_scene.isActive())
                        {
                            smpl::Camera& camera = m_main_scene.getCamera();

                            if (camera.getProjectionMode() == smpl::Camera::Projection::Perspective)
                            {
                                float camera_speed = 50.0f * m_state.delta_time;

                                if (keycode == smpl::Key::Code::W)
                                    camera.moveForward(camera_speed);
                                if (keycode == smpl::Key::Code::S)
                                    camera.moveForward(-camera_speed);
                                if (keycode == smpl::Key::Code::A)
                                    camera.moveRight(-camera_speed);
                                if (keycode == smpl::Key::Code::D)
                                    camera.moveRight(camera_speed);
                                if (keycode == smpl::Key::Code::Space)
                                    camera.moveUp(camera_speed);
                                if (keycode == smpl::Key::Code::LCtrl)
                                    camera.moveUp(-camera_speed);

                                float rot_speed = 50.0f * m_state.delta_time;
                                glm::vec3 rot_delta(0);

                                if (keycode == smpl::Key::Code::Up)
                                    rot_delta.x += rot_speed;
                                if (keycode == smpl::Key::Code::Down)
                                    rot_delta.x -= rot_speed;
                                if (keycode == smpl::Key::Code::Left)
                                    rot_delta.y += rot_speed;
                                if (keycode == smpl::Key::Code::Right)
                                    rot_delta.y -= rot_speed;

                                if (rot_delta != glm::vec3(0))
                                    camera.setRotation(camera.getRotation() + rot_delta);
                            }

                            if (camera.getProjectionMode() == smpl::Camera::Projection::Isometric)
                            {
                                float camera_speed = 30.0f * m_state.delta_time;
                                glm::vec3 pos = camera.getPosition();

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

                                camera.setPosition(pos);
                            }
                        }


                        if (keycode == smpl::Key::Code::F1)
                        {
                            if (!m_was_f1_pressed)
                            {
                                m_was_f1_pressed = true;

                                size_t ui_layer_index = 1;
                                auto layer = m_layer_manager.getLayer(ui_layer_index);
                                if (layer)
                                {
                                    if (m_ui_layer_visible)
                                    {
                                        m_layer_manager.hideLayer(ui_layer_index);
                                        m_ui_layer_visible = false;
                                    }
                                    else
                                    {
                                        m_layer_manager.showLayer(ui_layer_index);
                                        m_ui_layer_visible = true;
                                    }
                                }
                            }
                        }

                        if (keycode == smpl::Key::Code::Escape)
                        {
                            m_window->close();
                            return true;
                        }

                        return true;
                    });

            static bool first_mouse = true;
            static double last_mouse_x = 0.0;
            static double last_mouse_y = 0.0;

            dispatcher.dispatch<Core::KeyReleasedEvent>([this](Core::KeyReleasedEvent& e)
                    {
                        int keycode = e.getKeyCode();
                        if (keycode == smpl::Key::Code::F1)
                        {
                            m_was_f1_pressed = false;
                        }
                        return false;
                    });

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

                            if (m_main_scene.isActive())
                            {
                                smpl::Camera& camera = m_main_scene.getCamera();
                                if (camera.getProjectionMode() == smpl::Camera::Projection::Perspective)
                                {
                                    glm::vec3 current_rot = camera.getRotation();
                                    current_rot.y -= delta_x;
                                    current_rot.x += delta_y;

                                    current_rot.x = glm::clamp(current_rot.x, -89.0f, 89.0f);

                                    camera.setRotation(current_rot);
                                }
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

                        if (m_main_scene.isActive())
                        {
                            smpl::Camera& camera = m_main_scene.getCamera();
                            if (camera.getProjectionMode() == smpl::Camera::Projection::Perspective)
                            {
                                float rot_speed = 50.0f * m_state.delta_time;
                                camera.moveUp(scroll_y > 0 ? rot_speed : -rot_speed);
                            }
                            else if (camera.getProjectionMode() == smpl::Camera::Projection::Isometric)
                            {
                                float scroll_sensitivity = 1.0f;
                                float new_zoom = m_state.zoom - scroll_y * scroll_sensitivity;
                                m_state.zoom = glm::clamp(new_zoom, 1.0f, 100.0f);
                                camera.setZoom(m_state.zoom);
                            }
                        }
                        return true;
                    });

            return false;
        };
    }

    void Application::render()
    {
        m_window->clear(smpl::Color(50, 50, 50));

        // Update and render all layers
        m_layer_manager.updateAll(m_state.delta_time);
        m_layer_manager.renderAll();

        m_window->display();
    }

    bool Application::init()
    {
        m_settings.loadFromFile("config/editor_settings.json");
        m_window = std::make_unique<smpl::Window>(m_settings);

        if (!m_window->create())
            return false;

        if (!smpl::Gui::initImGui(*m_window))
            return false;

        if (!smpl::Gui::initImGuiFont())
            return false;

        setupEventCallbacks();

        if (!m_main_scene.load())
        {
            LOG_ERROR("Failed to load main scene");
            return false;
        }
        m_main_scene.activate();

        // Initialize layer system
        m_base_layer = std::make_shared<Editor::BaseLayer>();
        m_ui_layer   = std::make_shared<Editor::UILayer>();

        m_base_layer->SetCamera(m_main_scene.getCamera());
        m_ui_layer->setWindow(m_window.get());
        m_ui_layer->setDisplays(&m_displays);

        m_layer_manager.addLayer(m_base_layer);
        m_layer_manager.addLayer(m_ui_layer);

        m_layer_manager.initAll();

        // Activate both layers by default
        m_layer_manager.activateLayer(0);
        m_layer_manager.activateLayer(1);

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

            m_main_scene.getState().delta_time = m_state.delta_time;
            m_main_scene.update();

            render();
        }
    }
}