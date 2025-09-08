#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <array>

#include <Graphics/Shader/Shader.hpp>
#include <Graphics/VertexBuffer/VertexBuffer.hpp>
#include <Graphics/VertexArray/VertexArray.hpp>
#include <Window/Window.hpp>
#include <Graphics/Texture/Texture.hpp>

#include <Camera/Camera.hpp>


float camera_pos[3] = { 0.f, 0.f, -1.f };
float camera_rot[3] = { 0.f, 0.f, 0.f };
bool  perspective_camera = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float scale[3] = { 1.f, 1.f, 1.f };
float rotate = 0.f;
float translate[3] = { 0.f, 0.f, 0.f };


int main()
{
    smpl::Window window;

    smpl::VideoMode mode{ 1480 , 1200 };

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

    vertex_shader.loadFromFile("shaders/primitive_texture_shader.vert", smpl::Shader::Type::Vertex);
    fragment_shader.loadFromFile("shaders/primitive_texture_shader.frag", smpl::Shader::Type::Fragment);

    smpl::ShaderProgram program;
    program.create();
    program.bind(vertex_shader);
    program.bind(fragment_shader);

    if (!program.link())
        return -1;

    float vertices[] = {
        // front
        -1.0f, -1.f, -1.f,   1.f, 0.f,
        -1.0f,  1.f, -1.f,   0.f, 0.f,
        -1.0f, -1.f,  1.f,   1.f, 1.f,
        -1.0f,  1.f,  1.f,   0.f, 1.f,

        // back
         1.0f, -1.f, -1.f,   1.f, 0.f,
         1.0f,  1.f, -1.f,   0.f, 0.f,
         1.0f, -1.f,  1.f,   1.f, 1.f,
         1.0f,  1.f,  1.f,   0.f, 1.f
    };
    unsigned int indices[] = {
        0, 1, 2, 3, 2, 1, // front
        4, 5, 6, 7, 6, 5, // back
        0, 4, 6, 0, 2, 6, // right
        1, 5, 3, 3, 7, 5, // left
        3, 7, 2, 7, 6, 2, // top
        1, 5, 0, 5, 0, 4  // bottom
    };

    std::array<glm::vec3, 5> positions = {
               glm::vec3(-2.f, -2.f, -4.f),
               glm::vec3(-5.f,  0.f,  3.f),
               glm::vec3( 2.f,  1.f, -2.f),
               glm::vec3( 4.f, -3.f,  3.f),
               glm::vec3( 1.f, -7.f,  1.f)
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

    smpl::Event  event;
    smpl::Camera camera;

    camera.setViewportSize(1480, 1200);

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

            // TODO should improve input system
            // 
            //float cameraSpeed = static_cast<float>(2.5 * deltaTime);
            //if (event.key.code == smpl::Key::Code::W)
            //    camera_pos += cameraSpeed * cameraFront;
            //if (event.key.code == smpl::Key::Code::S)
            //    camera_pos -= cameraSpeed * cameraFront;
            //if (event.key.code == smpl::Key::Code::A)
            //    camera_pos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            //if (event.key.code == smpl::Key::Code::D)
            //    camera_pos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }

        float cameraSpeed = static_cast<float>(5.0f * deltaTime);

        if (glfwGetKey(&window.getWindow(), GLFW_KEY_Q) == GLFW_PRESS)
            camera_pos[2] -= cameraSpeed;
        if (glfwGetKey(&window.getWindow(), GLFW_KEY_E) == GLFW_PRESS)
            camera_pos[2] += cameraSpeed;
        if (glfwGetKey(&window.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
            camera_pos[0] -= cameraSpeed;
        if (glfwGetKey(&window.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
            camera_pos[0] += cameraSpeed;
        if (glfwGetKey(&window.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
            camera_pos[1] -= cameraSpeed;
        if (glfwGetKey(&window.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
            camera_pos[1] += cameraSpeed;

        window.clear(smpl::Color(30,30,30));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1.getTextureID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2.getTextureID());

        program.use();

        program.setUniformMatrix("view", camera.getViewMatrix());
        program.setUniformMatrix("projection", camera.getProjectionMatrix());

        for (const glm::vec3& current_position : positions)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), current_position);
            model = glm::translate(model, glm::vec3(translate[0], translate[1], translate[2]));
            model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(scale[0], scale[1], scale[2]));

            program.setUniformMatrix("model", model);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vao->getIndexesCount()), GL_UNSIGNED_INT, nullptr);
        }



        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Settings");

        ImGui::SliderFloat3("Scale", scale, 0.f, 2.f);
        ImGui::SliderFloat("Rotate", &rotate, 0.f, 360.f);
        ImGui::SliderFloat3("Translate", translate, -1.f, 1.f);
        
        ImGui::Separator();

        ImGui::SliderFloat3("camera position", camera_pos, -10.f, 10.f);
        ImGui::SliderFloat3("camera rotation", camera_rot, 0, 360.f);
        ImGui::Checkbox("Perspective camera", &perspective_camera);

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

        camera.setPositionAndRotation(glm::vec3(camera_pos[0], camera_pos[1], camera_pos[2]),
                                      glm::vec3(camera_rot[0], camera_rot[1], camera_rot[2]));
        camera.setProjection(perspective_camera ? smpl::Camera::Projection::Perspective : smpl::Camera::Projection::Orthographic);

        program.setUniformMatrix("projection", camera.getViewMatrix() * camera.getProjectionMatrix());

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