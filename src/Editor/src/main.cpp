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
#include <Graphics/Texture/Texture.hpp>

#include <Window/Window.hpp>
#include <Camera/Camera.hpp>
#include <ImGui/SetImGui.hpp>
#include <Log/Log.hpp>
#include <set>

#pragma region Variables

glm::vec3 camera_pos = { 0.f, -30.f, 15.f };
glm::vec3 camera_rot = { 0.f, -45.f,  0.f };

glm::vec3 scale      = { 1.f, 1.f, 1.f };
glm::vec3 translate  = { 0.f, 0.f, 0.f };
glm::vec3 rotate     = { 0.f, 0.f, 0.f };

glm::vec3 saved_perspective_pos = camera_pos;
glm::vec3 saved_perspective_rot = camera_rot;

bool  isometric_mode = false;

float delta_time = 0.0f;
float last_frame = 0.0f;
float angle      = 0.f;
float fov        = 0.f;

smpl::Camera camera (camera_pos, camera_rot);
smpl::Event  event;

#pragma endregion

void input(smpl::Window& window);
void initGUi(smpl::Window& window);

int main()
{
    smpl::Window window;

    smpl::VideoMode mode{ 2560 , 1600 };

    window.create(mode, "Medievalution");
    window.setVerticalSync(true);

    smpl::Gui::initImGui(window);
    smpl::Gui::initImGuiFont();

#pragma region Variables
    smpl::Shader fragment_shader;
    smpl::Shader vertex_shader;

    vertex_shader.  loadFromFile("shaders/primitive_texture_shader.vert", smpl::Shader::Type::Vertex);
    fragment_shader.loadFromFile("shaders/primitive_texture_shader.frag", smpl::Shader::Type::Fragment);

    smpl::ShaderProgram program;
    program.create();
    program.bind(vertex_shader);
    program.bind(fragment_shader);

    if (!program.link())
        return -1;

    smpl::Shader fragment_shader2;
    smpl::Shader vertex_shader2;

    vertex_shader2.  loadFromFile("shaders/grid_shader.vert", smpl::Shader::Type::Vertex);
    fragment_shader2.loadFromFile("shaders/grid_shader.frag", smpl::Shader::Type::Fragment);

    smpl::ShaderProgram program2;
    program2.create();
    program2.bind(vertex_shader2);
    program2.bind(fragment_shader2);

    if (!program2.link())
        return -1;

    float vertices[] = {
        // Front face
        -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,

        // Back face
        -1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
         1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
         1.0f,  1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,  1.0f, 1.0f,

        // Right face
         1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
         1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
         1.0f,  1.0f,  1.0f,  0.0f, 1.0f,

         // Left face
         -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
         -1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
         -1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
         -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,

         // Top face
         -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
          1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
          1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
         -1.0f,  1.0f,  1.0f,  0.0f, 0.0f,

         // Bottom face
         -1.0f, -1.0f,  1.0f,  0.0f, 1.0f,
          1.0f, -1.0f,  1.0f,  1.0f, 1.0f,
          1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
         -1.0f, -1.0f, -1.0f,  0.0f, 0.0f
    };

    unsigned int indices[] = {
        0,  1,  2,  0,  2,  3,  // front
        4,  5,  6,  4,  6,  7,  // back
        8,  9, 10,  8, 10, 11,  // right
        12, 13, 14, 12, 14, 15, // left
        16, 17, 18, 16, 18, 19, // top
        20, 21, 22, 20, 22, 23  // bottom
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
        smpl::ShaderDataType::Float3, // position
        smpl::ShaderDataType::Float2  // texture2D
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
    texture1.loadFromFile("res/stone_wall.png");

    smpl::Texture texture2;
    texture2.loadFromFile("res/horde.png");
#pragma endregion

    camera.setViewportSize(1480, 1200);

    while (window.isOpen())
    {
        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        while (window.pollEvent(event))
        {
            if (event.type == smpl::EventType::WindowClosed || event.key.code == smpl::Key::Code::Escape)
                window.close();
        }
        
        input(window);

        window.clear(smpl::Color(50,50,50));

#pragma region Cube
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1.getTextureID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2.getTextureID());

        program.use();

        program.setUniform1i("texture1", 0);
        program.setUniform1i("texture2", 1);
        program.setUniformMatrix("view", camera.getViewMatrix());
        program.setUniformMatrix("projection", camera.getProjectionMatrix());

        glm::mat4 base_transform = glm::mat4(1.0f);
        glm::mat4 rotation = glm::mat4(1.0f);

        rotation = glm::rotate(rotation, glm::radians(rotate.x), glm::vec3(1, 0, 0)); // Roll
        rotation = glm::rotate(rotation, glm::radians(rotate.y), glm::vec3(0, 1, 0)); // Pitch
        rotation = glm::rotate(rotation, glm::radians(rotate.z), glm::vec3(0, 0, 1)); // Yaw

        base_transform = glm::translate(glm::mat4(1.0f), translate);
        base_transform = base_transform * rotation;
        base_transform = glm::scale(base_transform, scale);

        for (const glm::vec3& pos : positions)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), pos) * base_transform;
            program.setUniformMatrix("model", model);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vao->getIndexesCount()), GL_UNSIGNED_INT, nullptr);
        }
#pragma endregion

#pragma region Grid
        glm::mat4 grid_transform = glm::mat4(1.0f);
        grid_transform = glm::rotate(grid_transform, glm::radians(0.0f), glm::vec3(1, 0, 0));
        grid_transform = glm::scale(grid_transform, glm::vec3(50.0f, 50.0f, 1.0f));

        glm::mat4 gridMVP = camera.getProjectionMatrix() * camera.getViewMatrix() * grid_transform;

        program2.use();
        program2.setUniformMatrix("grid_mvp", gridMVP);
        program2.setUniform1f("grid_step", 0.025f);
        program2.setUniform3f("grid_color", 0.6f, 0.6f, 0.6f);

        glDepthMask(GL_FALSE);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDepthMask(GL_TRUE);
#pragma endregion

        initGUi(window);

        window.display();
        glfwPollEvents();
    }

    smpl::Gui::destroyImGui();

    window.close();
    return 0;
}

void input(smpl::Window& window)
{
    if (camera.getProjectionMode() == smpl::Camera::Projection::Perspective)
    {
        float camera_speed = 30.0f * delta_time;

        if (glfwGetKey(&window.getWindow(), GLFW_KEY_W)            == GLFW_PRESS)
            camera.moveForward(camera_speed);                      
        if (glfwGetKey(&window.getWindow(), GLFW_KEY_S)            == GLFW_PRESS)
            camera.moveForward(-camera_speed);                     
        if (glfwGetKey(&window.getWindow(), GLFW_KEY_A)            == GLFW_PRESS)
            camera.moveRight(-camera_speed);                       
        if (glfwGetKey(&window.getWindow(), GLFW_KEY_D)            == GLFW_PRESS)
            camera.moveRight(camera_speed);                        
        if (glfwGetKey(&window.getWindow(), GLFW_KEY_SPACE)        == GLFW_PRESS)
            camera.moveUp(camera_speed);
        if (glfwGetKey(&window.getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            camera.moveUp(-camera_speed);

        float rot_speed = 50.0f * delta_time;
        glm::vec3 rot_delta(0);

        if (glfwGetKey(&window.getWindow(), GLFW_KEY_Q)     == GLFW_PRESS) rot_delta.x -= rot_speed;
        if (glfwGetKey(&window.getWindow(), GLFW_KEY_E)     == GLFW_PRESS) rot_delta.x += rot_speed;
        if (glfwGetKey(&window.getWindow(), GLFW_KEY_UP)    == GLFW_PRESS) rot_delta.y += rot_speed;
        if (glfwGetKey(&window.getWindow(), GLFW_KEY_DOWN)  == GLFW_PRESS) rot_delta.y -= rot_speed;
        if (glfwGetKey(&window.getWindow(), GLFW_KEY_LEFT)  == GLFW_PRESS) rot_delta.z += rot_speed;
        if (glfwGetKey(&window.getWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS) rot_delta.z -= rot_speed;

        if (rot_delta != glm::vec3(0))
            camera.setRotation(camera.getRotation() + rot_delta);

        // Field of view
        {
            if (glfwGetKey(&window.getWindow(), GLFW_KEY_EQUAL) == GLFW_PRESS)
                camera.setFieldOfView(camera.getFieldOfView() - camera_speed * delta_time);
            if (glfwGetKey(&window.getWindow(), GLFW_KEY_MINUS) == GLFW_PRESS)
                camera.setFieldOfView(camera.getFieldOfView() + camera_speed * delta_time);

            fov = camera.getFieldOfView();
            if (fov < 1.0f)  fov = 1.0f;
            if (fov > 90.0f) fov = 90.0f;
            camera.setFieldOfView(fov);
        }
    }

    if (camera.getProjectionMode() == smpl::Camera::Projection::Isometric)
    {
        float camera_speed = 30.0f * delta_time;

        glm::vec3 pos = camera.getPosition();

        if (glfwGetKey(&window.getWindow(), GLFW_KEY_A)            == GLFW_PRESS)
            pos.x -= camera_speed;                                 
        if (glfwGetKey(&window.getWindow(), GLFW_KEY_D)            == GLFW_PRESS)
            pos.x += camera_speed;                                 
        if (glfwGetKey(&window.getWindow(), GLFW_KEY_W)            == GLFW_PRESS)
            pos.y += camera_speed;                                 
        if (glfwGetKey(&window.getWindow(), GLFW_KEY_S)            == GLFW_PRESS)
            pos.y -= camera_speed;                                 
        if (glfwGetKey(&window.getWindow(), GLFW_KEY_SPACE)        == GLFW_PRESS)
            pos.z += camera_speed;
        if (glfwGetKey(&window.getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            pos.z -= camera_speed;

        camera.setPosition(pos);
    }
}

void initGUi(smpl::Window& window)
{
    smpl::Gui::initBackEndImGui();
    ImGui::NewFrame();

    static bool show_settings_window;
    static bool show_navigation_window;
    static bool show_demo;
    static bool fullscreen;

    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("File"))
    {
        ImGui::MenuItem("New",  "Ctrl+N");
        ImGui::MenuItem("Open", "Ctrl+O");
        ImGui::MenuItem("Save", "Ctrl+S");
        ImGui::MenuItem("Save As..", "Ctrl+Shift+S");

        ImGui::Separator();

        if (ImGui::MenuItem("Exit", "Alt+F4"))
            window.close();

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
        ImGui::EndMenu();

    if (ImGui::BeginMenu("Settings"))
    {
       ImGui::MenuItem("Settings",   nullptr, &show_settings_window);
       ImGui::MenuItem("Navigation", nullptr, &show_navigation_window);
       ImGui::MenuItem("Demo",       nullptr, &show_demo);
       ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Help"))
        ImGui::EndMenu();

    ImGui::EndMainMenuBar();

    if (show_demo)
        ImGui::ShowDemoWindow();

    // Settings Window
    if (show_navigation_window)
    {

        glm::vec3 cam_pos = camera.getPosition();
        glm::vec3 cam_rot = camera.getRotation();
        
        ImGui::Begin("Navigation", &show_navigation_window, ImGuiChildFlags_AlwaysAutoResize);

        ImGui::SeparatorText("Navigation");

        if (ImGui::SliderFloat3("Camera position", glm::value_ptr(cam_pos), -100.0f, 100.0f))
            camera.setPosition(cam_pos);

        if (!isometric_mode)
        {
            if (ImGui::SliderFloat3("Camera rotation", glm::value_ptr(cam_rot), 0.0f, 360.0f))
                camera.setRotation(cam_rot);

            if (ImGui::SliderFloat("Field of view", &fov, 1.0f, 90.0f))
                camera.setFieldOfView(fov);
        }
        else
        {
            auto zoom = camera.getZoom();

            if (ImGui::SliderFloat("Zoom", &zoom, 1.0f, 90.0f))
                camera.setZoom(zoom);
        }

        if (ImGui::Checkbox("Isometric", &isometric_mode))
        {
            if (isometric_mode)
            {
                saved_perspective_pos = camera.getPosition();
                saved_perspective_rot = camera.getRotation();

                camera.setRotation(glm::vec3(-45.0f, -45.0f, 0.0f));
                camera.setProjection(smpl::Camera::Projection::Isometric);
            }
            else
            {
                camera.setPosition(saved_perspective_pos);
                camera.setRotation(saved_perspective_rot);
                camera.setProjection(smpl::Camera::Projection::Perspective);
            }

        }

        ImGui::Dummy(ImVec2(0, 30));
        ImGui::SeparatorText("Object");

        ImGui::SliderFloat3("Scale",     glm::value_ptr(scale),        0.5f,  2.0f);
        ImGui::SliderFloat3("Translate", glm::value_ptr(translate), -100.0f, 100.0f);
        ImGui::SliderFloat3("Rotate",    glm::value_ptr(rotate),       0.0f, 360.0f);

        ImGui::Dummy(ImVec2(0, 30));

        ImGui::End();
    }

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

        if (ImGui::Button("Applay"))
        {
            const smpl::VideoMode& mode = m_available_modes[m_selected_index];
            window.setVideoMode(mode, fullscreen);
            LOG_INFO("Resolution: {0}x{1}", mode.width, mode.height);
        }

        ImGui::End();
    }

    smpl::Gui::drawImGuiGL();
}
