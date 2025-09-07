#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <Graphics/Shader/Shader.hpp>
#include <Graphics/VertexBuffer/VertexBuffer.hpp>
#include <Graphics/VertexArray/VertexArray.hpp>
#include <Window/Window.hpp>
#include <Graphics/Texture/Texture.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float scale[3] = { 1.f, 1.f, 1.f };
float rotate = 0.f;
float translate[3] = { 0.f, 0.f, 0.f };

int main()
{
    smpl::Window window;

    smpl::VideoMode mode{ 800 , 600 };

    window.create(mode, "Medievalution");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(&window.getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig config;
    config.SizePixels = 25.0f;
    io.Fonts->AddFontDefault(&config);


    smpl::Shader fragment_shader;
    smpl::Shader vertex_shader;

    fragment_shader.loadFromFile("shaders/primitive_texture_shader.vert", smpl::Shader::Type::Vertex);
    vertex_shader.  loadFromFile("shaders/primitive_texture_shader.frag", smpl::Shader::Type::Fragment);

    smpl::ShaderProgram program;
    program.create();
    program.bind(vertex_shader);
    program.bind(fragment_shader);

    if (!program.link())
        return -1;

    float vertices[] = {
        // positions          // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    smpl::BufferLayout layout
    {
        smpl::ShaderDataType::Float3,       // position
        smpl::ShaderDataType::Float2        // texture2D
    };

    std::unique_ptr<smpl::VertexBuffer> vbo;
    std::unique_ptr<smpl::VertexArray>  vao;
    std::unique_ptr<smpl::IndexBuffer>  index_buffer;

    vao          = std::make_unique<smpl::VertexArray>();
    vbo          = std::make_unique<smpl::VertexBuffer>(vertices, sizeof(vertices), layout);
    index_buffer = std::make_unique<smpl::IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));

    vao->addVertexBuffer(*vbo);
    vao->setIndexBuffer(*index_buffer);

    smpl::Texture texture1;
    texture1.loadFromFile("res/rus.png");

    smpl::Texture texture2;
    texture2.loadFromFile("res/ussr.png");

    program.use();
    program.setUniform1i("texture1", 0);
    program.setUniform1i("texture2", 1);

    smpl::Event event;

    while (window.isOpen())
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        while (window.pollEvent(event))
        {
            if (event.type == smpl::EventType::WindowClosed || event.key.code == smpl::Key::Code::Escape)
            {
                window.close();
            }

            float cameraSpeed = static_cast<float>(2.5 * deltaTime);
            if (event.key.code == smpl::Key::Code::W)
                cameraPos += cameraSpeed * cameraFront;
            if (event.key.code == smpl::Key::Code::S)
                cameraPos -= cameraSpeed * cameraFront;
            if (event.key.code == smpl::Key::Code::A)
                cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            if (event.key.code == smpl::Key::Code::D)
                cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }

        window.clear();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1.getTextureID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2.getTextureID());

        program.use();

        glm::mat4 scale_matrix(scale[0], 0, 0, 0,
            0, scale[1], 0, 0,
            0, 0, scale[2], 0,
            0, 0, 0, 1);

        float rotate_in_radians = glm::radians(rotate);
        glm::mat4 rotate_matrix(cos(rotate_in_radians), sin(rotate_in_radians), 0, 0,
            -sin(rotate_in_radians), cos(rotate_in_radians), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);

        glm::mat4 translate_matrix(1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            translate[0], translate[1], translate[2], 1);

        glm::mat4 model_matrix = translate_matrix * rotate_matrix * scale_matrix;
        glm::mat4 projection   = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 100.0f);

        program.setUniformMatrix("model_matrix", model_matrix);
        //program.setUniformMatrix("projection", projection);

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vao->getIndexesCount()), GL_UNSIGNED_INT, nullptr);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Settings");
        ImGui::SliderFloat3("Scale", scale, 0.f, 2.f);
        ImGui::SliderFloat("Rotate", &rotate, 0.f, 360.f);
        ImGui::SliderFloat3("Translate", translate, -1.f, 1.f);
        ImGui::End();

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.display();
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    window.close();
    return 0;
}