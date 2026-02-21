#include "TabMenuDisplay.hpp"

#include <imgui.h>

#include <vector>
#include <string>
#include <set>
#include <memory>
#include <Log/Log.hpp>

namespace Editor
{
    TabMenuDisplay::TabMenuDisplay()
        : m_show_settings(false)
    {
    }

    void TabMenuDisplay::show()
    {
        static char filePath[256] = "map_save.json";

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New", "Ctrl+N")) {}

                if (ImGui::MenuItem("Open", "Ctrl+O")) {}

                if (ImGui::MenuItem("Save", "Ctrl+S")) {}

                if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S")) {}

                ImGui::Separator();

                if (ImGui::MenuItem("Exit", "Alt+F4")) { /*window.close(); */ }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}

                ImGui::Separator();

                if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View"))
            {
                ImGui::SeparatorText("Editors");

                ImGui::SeparatorText("Other");

                if (ImGui::MenuItem("Settings"))
                {
                    m_show_settings = true;
                }

                ImGui::MenuItem("Demo Window", nullptr, &show_demo_window);

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        if (m_show_settings)
        {
            ImGui::OpenPopup("Settings##");
            m_show_settings = false;
        }

        showSettings();
    }

    void TabMenuDisplay::showSettings()
    {
        if (ImGui::BeginPopupModal("Settings##", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))
        {
            ImGui::SeparatorText("Window");

            ImGui::Text("Resolution");
            ImGui::Text("Fullscreen");

            ImGui::SeparatorText("Audio");

            ImGui::Text("Music volume");
            ImGui::Text("Sound volume");

            ImGui::Spacing();

            if (ImGui::Button("Accept"))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Delay"))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        /*
        if (show_settings_window)
        {
            std::vector<smpl::VideoMode> m_available_modes;
            std::vector<std::string>     m_mode_labels;

            if (!m_available_modes.empty())
                return;

            auto modes = smpl::VideoMode::getFullscreenModes();

            std::set<std::pair<int, int>> seen;
            std::vector<smpl::VideoMode> unique;

            for (const auto& mode : modes)
            {
                auto key = std::make_pair(mode.width, mode.height);
                if (seen.insert(key).second)
                    unique.push_back(mode);
            }

            std::sort(unique.begin(), unique.end(), [](const smpl::VideoMode& a, const smpl::VideoMode& b)
                { return (a.width * a.height) > (b.width * b.height); });

            m_available_modes = std::move(unique);

            m_mode_labels.clear();

            for (const auto& mode : m_available_modes)
            {
                m_mode_labels.push_back(std::to_string(mode.width) + "x" + std::to_string(mode.height));
            }

            static int m_selected_index = 0;

            ImGui::Begin("Settings", &show_settings_window);

            if (ImGui::BeginCombo("Resolution", m_mode_labels[m_selected_index].c_str()))
            {
                for (int i = 0; i < m_mode_labels.size(); i++)
                {
                    bool selected = (i == m_selected_index);
                    if (ImGui::Selectable(m_mode_labels[i].c_str(), selected))
                        m_selected_index = i;

                    if (selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            ImGui::Checkbox("Fullscreen", &fullscreen);

            if (ImGui::Button("Apply"))
            {
                const smpl::VideoMode& mode = m_available_modes[m_selected_index];
                window.setVideoMode(mode, fullscreen);
                LOG_INFO("Resolution: {0}x{1}", mode.width, mode.height);
            }

            ImGui::End();
        }
        */
    }
}