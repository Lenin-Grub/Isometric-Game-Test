#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Log/Log.hpp>

#include <Graphics/Shader/Shader.hpp>
#include <Graphics/Texture/Texture.hpp>
#include <Graphics/Color/Color.hpp>
#include <Graphics/VertexBuffer/VertexBuffer.hpp>
#include <Graphics/VertexArray/VertexArray.hpp>
#include <Graphics/IndexBuffer/IndexBuffer.hpp>

#include <Window/Window.hpp>

class Game
{
public:
	Game()  = default;
	~Game() = default;

	bool create();

	void run();

private:

    bool init();
    void input();
    void draw();
    void update();

    void close();


private:
	const unsigned int WIDTH = 1280;
	const unsigned int HEIGHT = 920;

	smpl::Window window;
    smpl::Event  event;
    smpl::Color  color{ 50,50,50 };

private:
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
};