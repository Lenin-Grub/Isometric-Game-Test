#include "UILayer.hpp"
#include <Log/Log.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Editor
{

    UILayer::UILayer() : m_window(nullptr), m_displays(nullptr)
    {
        LOG_INFO("UILayer created");
    }

    bool UILayer::init()
    {
        return false;
    }

    void UILayer::update(float deltaTime)
    {
        if (!isActive() || !isVisible() || !m_window || !m_displays)
        {
            return;
        }
    }

    void UILayer::render()
    {
        if (!isVisible() || !m_window || !m_displays)
        {
            return;
        }

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Render UI displays
        m_displays->show();

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

}
