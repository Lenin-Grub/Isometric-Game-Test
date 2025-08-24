#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Window/Window.hpp>
#include <Log/Log.hpp>

#include <stb_image.h>

#include <Graphics/Shader/Shader.hpp>

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

#pragma region Shader

    smpl::Shader vertex_shader;
    if (!vertex_shader.loadFromFile("shaders/primitive_texture_shader.vert", smpl::ShaderType::Vertex))
    {
        return -1;
    }

    smpl::Shader fragment_shader;
    if (!fragment_shader.loadFromFile("shaders/primitive_texture_shader.frag", smpl::ShaderType::Fragment))
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

    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
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

    //-----------
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    int width, height, nrChannels;
    unsigned char* data = stbi_load("D:/Repositories/Medievalution3D/res/rus.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        stbi__vertical_flip(data, width, height, nrChannels);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

#pragma endregion

    smpl::Color color{ 50,50,50 };

    while (!glfwWindowShouldClose(&window.getWindow()))
    {
        window.clear(color);

        initBackEndImGui();
        
        program.use();
        glBindVertexArray(VAO);
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
