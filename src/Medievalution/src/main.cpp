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
#include <Graphics/IndexBuffer/IndexBuffer.hpp>

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

    LogInfo::initLogger();

    float vertices[] = {
        // positions          // colors           //// texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };

    unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
    };

#pragma region Render with Vertex Buffer

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

    smpl::BufferLayout layout
    {
        smpl::ShaderDataType::Float3,       // position
        smpl::ShaderDataType::Float3,       // color
        smpl::ShaderDataType::Float2        // texture2D
    };

    std::unique_ptr<smpl::VertexBuffer> vbo;
    std::unique_ptr<smpl::VertexArray>  vao;
    std::unique_ptr<smpl::IndexBuffer>  index_buffer;

    vao          = std::make_unique<smpl::VertexArray>();
    vbo          = std::make_unique<smpl::VertexBuffer>(vertices, sizeof(vertices), layout);
    index_buffer = std::make_unique<smpl::IndexBuffer>(indices,sizeof(indices) / sizeof(GLuint) );


    vao->addVertexBuffer(*vbo);
    vao->setIndexBuffer(*index_buffer);

    smpl::Texture texture;
    if (!texture.loadFromFile("res/rus.png"))
        return -1;

#pragma endregion

    smpl::Color color{ 50,50,50 };

    while (!glfwWindowShouldClose(&window.getWindow()))
    {
        window.clear(color);

        initBackEndImGui();

        program.use();
        vao->bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vao->getIndexesCount()), GL_UNSIGNED_INT, nullptr);

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
