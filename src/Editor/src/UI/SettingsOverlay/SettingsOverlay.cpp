#include "SettingsOverlay.hpp"

namespace Editor
{
    void SettingsOverlay::show()
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration          |
                                 ImGuiWindowFlags_NoMove                |
                                 ImGuiWindowFlags_NoResize              |
                                 ImGuiWindowFlags_NoBringToFrontOnFocus |
                                 ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));

        if (ImGui::Begin("##Overlay", nullptr, flags))
        {
            ImGui::SetCursorPos(ImVec2(20, 20));
            ImGui::Text("Overlay Content");
            ImGui::Separator();
            if (ImGui::Button("Close")) { /* ... */ }

            ImGui::SetCursorPos(ImVec2(
                (viewport->Size.x - 300) * 0.5f,
                (viewport->Size.y - 200) * 0.5f
            ));
            ImGui::BeginChild("Popup", ImVec2(300, 200), true, ImGuiWindowFlags_None);
            ImGui::Text("This is an overlay.");
            ImGui::EndChild();
        }
        ImGui::End();

        ImGui::PopStyleColor();
    }
}