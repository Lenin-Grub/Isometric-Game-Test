#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Window/Window.hpp>
#include <Log/Log.hpp>

#include <Graphics/Shader/Shader.hpp>

GLfloat vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
};

GLfloat colors [] = {
    1.0f,  0.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
    0.0f,  0.0f, 1.0f,
    0.0f,  0.0f, 1.0f,
};

GLuint indices[] = {
    0, 1, 3,
    1, 2, 3
};

bool initImGui(const smpl::Window& window)
{
    if (!&window.getWindow())
    {
        LOG_CRITICAL("Window is nullptr!");
        return false;
    }

    if(!IMGUI_CHECKVERSION())
    {
        LOG_CRITICAL("Wrong version of ImGui!");
        return false;
    }

    if(!ImGui::CreateContext())
    {
        LOG_CRITICAL("Failed create context to ImGui!");
        return false;
    }

    if(!ImGui_ImplGlfw_InitForOpenGL(&window.getWindow(), true))
    {
        LOG_CRITICAL("Failed init GLFW to ImGui!");
        return false;
    }

    if (!ImGui_ImplOpenGL3_Init("#version 460"))
    {
        LOG_CRITICAL("Failed init opengl3 to ImGui!");
        return false;
    }

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


int main()
{
    const unsigned int WIDTH  = 1280;
    const unsigned int HEIGHT = 920;

    smpl::Window window;
    window.create(WIDTH, HEIGHT, "Medievalution");

    initImGui(window);

    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig config;
    config.SizePixels = 25.0f;
    io.Fonts->AddFontDefault(&config);

    LogInfo::initLogger();

#pragma region Shader

    smpl::Shader vertex_shader;
    if (!vertex_shader.loadFromFile("shaders/shader.vert", GL_VERTEX_SHADER))
    {
        return -1;
    }

    smpl::Shader fragment_shader;
    if (!fragment_shader.loadFromFile("shaders/shader.frag", GL_FRAGMENT_SHADER))
    {
        return -1;
    }

    smpl::ShaderProgram program;
    program.create();
    program.bind(vertex_shader);
    program.bind(fragment_shader);

    if (!program.link())
    {
        return -1;
    }

    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint colors_vbo = 0;
    glGenBuffers(1, &colors_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    GLuint ebo = 0;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

#pragma endregion

    smpl::Color color{ 50,50,50 };

    while (!glfwWindowShouldClose(&window.getWindow()))
    {
        window.clear(color);

        initBackEndImGui();
        
        program.use();
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        ImGui::NewFrame();

        //ImGui::ShowDemoWindow();
        
        ImGui::Render();
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwPollEvents();
        window.processInput(&window.getWindow());

        window.display();
    }

    destroyImGui();

    window.close();
    return 0;
}
