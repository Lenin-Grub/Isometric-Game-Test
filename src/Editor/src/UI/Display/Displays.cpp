#include "Displays.hpp"
#include <imgui.h>
#include <imgui_internal.h>

namespace Editor
{
    Displays::Displays()
        : m_docking_layout_initialized(false)
    {
    }

    bool Displays::initDockSpace()
    {
        static bool dockspace_open = true;
        static bool opt_fullscreen = true;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                            ImGuiWindowFlags_NoResize   | ImGuiWindowFlags_NoMove     |
                            ImGuiWindowFlags_NoBackground;

            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus ;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

        ImGui::Begin("DockSpace", &dockspace_open, window_flags);

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            ImGui::PopStyleVar();

         ImGui::PopStyleColor(2);

        return true;
    }

    bool Displays::create()
    {
        m_tab_menu.show();
        m_display_log.show();
        m_object_details.show();
        m_scene_hierarchy.show();
        m_view_space.show();
        m_assets_display.show();
        m_compass.show();

        if (m_tab_menu.show_demo_window)
            ImGui::ShowDemoWindow(&m_tab_menu.show_demo_window);

        return true;
    }

    void Displays::show()
    {
        initDockSpace();
        initDockingLayout();
        create();

        ImGui::End();
    }

    void Displays::initDockingLayout()
    {
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dock_space_id = ImGui::GetID("DockSpace");

            ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::DockSpace(dock_space_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
            ImGui::PopStyleColor();

            static auto first_time = true;

            if (first_time)
            {
                first_time = false;

                ImGui::DockBuilderRemoveNode(dock_space_id);
                ImGui::DockBuilderAddNode(dock_space_id, ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dock_space_id, ImGui::GetMainViewport()->Size);

                auto center       = dock_space_id;
                auto left         = ImGui::DockBuilderSplitNode(center, ImGuiDir_Left,  0.20f,  nullptr, &center);
                auto right        = ImGui::DockBuilderSplitNode(center, ImGuiDir_Right, 0.25f,  nullptr, &center);
                auto bottom       = ImGui::DockBuilderSplitNode(center, ImGuiDir_Down,  0.25f,  nullptr, &center);
                auto right_bottom = ImGui::DockBuilderSplitNode(right,  ImGuiDir_Down,  0.25f,  nullptr, &right);
                auto top          = ImGui::DockBuilderSplitNode(center, ImGuiDir_Up,    0.20f,  nullptr, &center);

                ImGui::DockBuilderDockWindow("Compass",         top);
                ImGui::DockBuilderDockWindow("Scene Hierarchy", left);
                ImGui::DockBuilderDockWindow("Inspector",       right);
                ImGui::DockBuilderDockWindow("Object Details",  right);
                ImGui::DockBuilderDockWindow("Scene",           right_bottom);
                ImGui::DockBuilderDockWindow("Logs",            bottom);
                ImGui::DockBuilderDockWindow("Assets",          bottom);
                ImGui::DockBuilderFinish(dock_space_id);
            }
        }
    }
}