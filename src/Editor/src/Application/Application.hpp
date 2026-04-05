#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <array>
#include <set>
#include <memory>

#include <Window/Window.hpp>
#include <Event/Input.hpp>
#include <ImGui/SetImGui.hpp>
#include <Log/Log.hpp>
#include <Settings/Settings.hpp>

#include "../UI/Display/Displays.hpp"
#include "../Layer/LayerManager.hpp"
#include "../Layer/BaseLayer.hpp"
#include "../Layer/UILayer.hpp"

namespace Editor
{   
class Application
    {
    public:
        Application();
        ~Application() = default;
        bool init();
        void run();

    private:
        void setupEventCallbacks();

        void render();

    private:
        smpl::Settings                     m_settings;
        std::unique_ptr<smpl::Window>      m_window;

        Editor::LayerManager               m_layer_manager;
        std::shared_ptr<Editor::BaseLayer> m_base_layer;
        std::shared_ptr<Editor::UILayer>   m_ui_layer;

    struct GlobalState
    {
        glm::vec3 camera_pos = {  0.f, -30.f, 15.f };
        glm::vec3 camera_rot = { 45.f,  90.f,  0.f };

        smpl::Camera camera { camera_pos, camera_rot };

        glm::vec3 scale      = { 1.f, 1.f, 1.f };
        glm::vec3 translate  = { 0.f, 0.f, 0.f };
        glm::vec3 rotate     = { 0.f, 0.f, 0.f };

        glm::vec3 saved_perspective_pos = camera_pos;
        glm::vec3 saved_perspective_rot = camera_rot;

        bool  isometric_mode = false;

        float delta_time = 0.0f;
        float last_frame = 0.0f;
        float angle      = 0.f;
        float fov        = 0.f;
        float zoom       = 0.f;
    };

        GlobalState             m_state;
        Editor::Displays        m_displays;
        bool m_was_f1_pressed   = false;
        bool m_ui_layer_visible = true;
    };
}