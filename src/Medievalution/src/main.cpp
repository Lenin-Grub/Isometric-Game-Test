#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Log/Log.hpp>

#include <Graphics/Shader/Shader.hpp>
#include <Graphics/Texture/Texture.hpp>
#include <Graphics/Color/Color.hpp>
#include <Graphics/VertexBuffer/VertexBuffer.hpp>
#include <Graphics/VertexArray/VertexArray.hpp>

#include <Window/Window.hpp>

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

    //LogInfo::initLogger();

    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };

    float vertices_triangle[] = {
        // positions       
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
    };

    float vertices_color[] = {
        // positions       
         1.0f,  0.0f, 0.0f,
         1.0f,  0.0f, 0.0f,
         1.0f,  0.0f, 0.0f,
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

#pragma region Render with Vertex Buffer

    smpl::Shader vertex_shader2;
    if (!vertex_shader2.loadFromFile("shaders/primitive_color_shader.vert", smpl::Shader::Type::Vertex))
        return -1;

    smpl::Shader fragment_shader2;
    if (!fragment_shader2.loadFromFile("shaders/primitive_color_shader.frag", smpl::Shader::Type::Fragment))
        return -1;

    smpl::ShaderProgram program2;
    program2.create();
    program2.bind(vertex_shader2);
    program2.bind(fragment_shader2);

    if (!program2.link())
        return -1;

    std::unique_ptr<smpl::VertexBuffer> points_vbo;
    std::unique_ptr<smpl::VertexBuffer> points_color;
    std::unique_ptr<smpl::VertexArray>  vao;

    points_vbo   = std::make_unique<smpl::VertexBuffer>(vertices_triangle, sizeof(vertices_triangle));
    points_color = std::make_unique<smpl::VertexBuffer>(vertices_color,    sizeof(vertices_color));
    vao          = std::make_unique<smpl::VertexArray>();

    vao->addBuffer(*points_vbo);
    vao->addBuffer(*points_color);
#pragma endregion

#pragma region Render clean opengl
    smpl::Shader vertex_shader;
    if (!vertex_shader.loadFromFile("shaders/primitive_texture_shader.vert", smpl::Shader::Type::Vertex))
        return -1;

    smpl::Shader fragment_shader;
    if (!fragment_shader.loadFromFile("shaders/primitive_texture_shader.frag", smpl::Shader::Type::Fragment))
        return -1;

    smpl::ShaderProgram program;
    program.create();
    program.bind(vertex_shader);
    program.bind(fragment_shader);

    if (!program.link())
        return -1;

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    smpl::Texture texture;
    if (!texture.loadFromFile("D:/Repositories/Medievalution3D/res/rus.png"))
        return -1;
    
    //if (!texture.loadFromFile("D:/Repositories/Medievalution3D/res/eng.png"))
    //    return -1;

#pragma endregion

    smpl::Color color{ 50,50,50 };

    while (!glfwWindowShouldClose(&window.getWindow()))
    {
        window.clear(color);

        initBackEndImGui();
       
        // clean opengl
        program.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // incapsulate VAO
        program2.use();
        vao->bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);

        ImGui::NewFrame();

        //ImGui::ShowDemoWindow();
        
        ImGui::Begin("Color background");
        ImGui::ColorPicker3("Color", color.data());
        ImGui::End();

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
