#include <ImGui/SetImGui.hpp>
#include <Log/Log.hpp>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Window/Window.hpp>

namespace smpl
{
    namespace Gui
    {
        bool initImGui(const smpl::Window& window)
        {
            if (!&window.getWindow())
            {
                LOG_CRITICAL("Window is nullptr!");
                return false;
            }

            if (!IMGUI_CHECKVERSION())
            {
                LOG_CRITICAL("Wrong version of ImGui!");
                return false;
            }

            if (!ImGui::CreateContext())
            {
                LOG_CRITICAL("Failed create context to ImGui!");
                return false;
            }

            if (!ImGui_ImplGlfw_InitForOpenGL(&window.getWindow(), true))
            {
                LOG_CRITICAL("Failed init GLFW to ImGui!");
                return false;
            }

            if (!ImGui_ImplOpenGL3_Init("#version 460"))
            {
                LOG_CRITICAL("Failed init opengl3 to ImGui!");
                return false;
            }

            LOG_DEBUG("ImGui inited");

            return true;
        }

        bool destroyImGui()
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();

            return true;
        }

        bool initBackEndImGui()
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            return true;
        }

        bool drawImGuiGL()
        {
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            return true;
        }

        bool initImGuiFont()
        {
            ImGuiIO& io = ImGui::GetIO();
            ImFontConfig config;
            config.SizePixels = 25.0f;
            io.Fonts->AddFontDefault(&config);
            return true;
        }
    }
}