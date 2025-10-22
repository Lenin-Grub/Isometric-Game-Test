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


float camera_pos[3] = { -30.f, 15.f, 0.f };
float camera_rot[3] = { 90.f, 0.f, -45.f };
bool  perspective_camera = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float scale[3] = { 1.f, 1.f, 1.f };
float rotate = 0.f;
float translate[3] = { 0.f, 0.f, 0.f };

void input(smpl::Window& window);
void initGUi(smpl::Window& window);

bool mouse_captured = false;
double last_mouse_x = 0.0, last_mouse_y = 0.0;
float yaw = 0.0f;
float pitch = 0.0f;

int main()
{
    smpl::Window window;

    smpl::VideoMode mode{ 2560 , 1600 };

    window.create(mode, "Medievalution");
    window.setVerticalSync(true);

    smpl::Gui::initImGui(window);
    smpl::Gui::initImGuiFont();

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

    vertex_shader2.loadFromFile("shaders/grid_shader.vert", smpl::Shader::Type::Vertex);
    fragment_shader2.loadFromFile("shaders/grid_shader.frag", smpl::Shader::Type::Fragment);

    smpl::ShaderProgram program2;
    program2.create();
    program2.bind(vertex_shader2);
    program2.bind(fragment_shader2);

    if (!program2.link())
        return -1;

    float vertices[] = {
        // Front face
        -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,  // bottom-left
         1.0f, -1.0f,  1.0f,  1.0f, 0.0f,  // bottom-right
         1.0f,  1.0f,  1.0f,  1.0f, 1.0f,  // top-right
        -1.0f,  1.0f,  1.0f,  0.0f, 1.0f,  // top-left

        // Back face
        -1.0f, -1.0f, -1.0f,  1.0f, 0.0f,  // bottom-right (mirrored)
         1.0f, -1.0f, -1.0f,  0.0f, 0.0f,  // bottom-left
         1.0f,  1.0f, -1.0f,  0.0f, 1.0f,  // top-left
        -1.0f,  1.0f, -1.0f,  1.0f, 1.0f,  // top-right

        // Right face
         1.0f, -1.0f, -1.0f,  0.0f, 0.0f,  // bottom-left
         1.0f, -1.0f,  1.0f,  1.0f, 0.0f,  // bottom-right
         1.0f,  1.0f,  1.0f,  1.0f, 1.0f,  // top-right
         1.0f,  1.0f, -1.0f,  0.0f, 1.0f,  // top-left

         // Left face
         -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,  // bottom-left
         -1.0f, -1.0f, -1.0f,  1.0f, 0.0f,  // bottom-right
         -1.0f,  1.0f, -1.0f,  1.0f, 1.0f,  // top-right
         -1.0f,  1.0f,  1.0f,  0.0f, 1.0f,  // top-left

         // Top face
         -1.0f,  1.0f,  1.0f,  0.0f, 1.0f,  // top-left
          1.0f,  1.0f,  1.0f,  1.0f, 1.0f,  // top-right
          1.0f,  1.0f, -1.0f,  1.0f, 0.0f,  // bottom-right
         -1.0f,  1.0f, -1.0f,  0.0f, 0.0f,  // bottom-left

         // Bottom face
         -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,  // top-left
          1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  // top-right
          1.0f, -1.0f,  1.0f,  1.0f, 0.0f,  // bottom-right
         -1.0f, -1.0f,  1.0f,  0.0f, 0.0f   // bottom-left
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
    texture1.loadFromFile("res/stone_wall.png");

    smpl::Texture texture2;
    texture2.loadFromFile("res/horde.png");

    //program.use();
    //program.setUniform1i("texture1", 0);
    //program.setUniform1i("texture2", 1);

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
        }
        
        input(window);

        window.clear(smpl::Color(50,50,50));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1.getTextureID());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2.getTextureID());

        program.use();

        program.setUniform1i("texture1", 0);
        program.setUniform1i("texture2", 1);
        program.setUniformMatrix("view", camera.getViewMatrix());
        program.setUniformMatrix("projection", camera.getProjectionMatrix());

        glm::mat4 baseTransform = glm::mat4(1.0f);
        baseTransform = glm::translate(baseTransform, glm::vec3(translate[0], translate[1], translate[2]));
        baseTransform = glm::scale(baseTransform, glm::vec3(scale[0], scale[1], scale[2]));
        baseTransform = glm::rotate(baseTransform, glm::radians(rotate), glm::vec3(0, 0, 1));

        for (const glm::vec3& pos : positions)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), pos) * baseTransform;
            program.setUniformMatrix("model", model);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vao->getIndexesCount()), GL_UNSIGNED_INT, nullptr);
        }

        //_______GRID_______
        glm::mat4 gridTransform = glm::mat4(1.0f);
        //gridTransform = glm::translate(gridTransform, glm::vec3(translate[0], 0.0f, translate[2])); // ← Y = 0, если сетка на полу
        gridTransform = glm::rotate(gridTransform, glm::radians(0.0f), glm::vec3(1, 0, 0));
        gridTransform = glm::scale(gridTransform, glm::vec3(50.0f, 1.0f, 50.0f)); // ← Y масштаб = 1.0!

        glm::mat4 gridMVP = camera.getProjectionMatrix() * camera.getViewMatrix() * gridTransform;

        program2.use();
        program2.setUniformMatrix("grid_mvp", gridMVP);
        program2.setUniform1f("grid_step", 0.025f);
        program2.setUniform3f("grid_color", 0.6f, 0.6f, 0.6f);
        //program2.setUniform3f("axis_color", 1.0f, 1.0f, 0.0f);
        glDepthMask(GL_FALSE);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDepthMask(GL_TRUE);
        //_______GRID_______

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
    float cameraSpeed = static_cast<float>(10.0f * deltaTime);
    
    static bool was_pressed = false;
    bool is_pressed = glfwGetMouseButton(&window.getWindow(), GLFW_MOUSE_BUTTON_2) == GLFW_PRESS;

    if (is_pressed)
    {
        double x, y;
        glfwGetCursorPos(&window.getWindow(), &x, &y);

        if (!was_pressed)
        {
            last_mouse_x = x;
            last_mouse_y = y;
        }
        else
        {
            float dx = static_cast<float>(x - last_mouse_x);
            float dy = static_cast<float>(y - last_mouse_y);
            last_mouse_x = x;
            last_mouse_y = y;

            const float sens = 0.1f;
            yaw -= dx * sens;
            pitch -= dy * sens;
            pitch = glm::clamp(pitch, -89.0f, 89.0f);

            camera_rot[2] = yaw;
            camera_rot[1] = pitch;
        }
    }

    was_pressed = is_pressed;

    if (glfwGetKey(&window.getWindow(), GLFW_KEY_Q) == GLFW_PRESS)
        camera_rot[0] -= cameraSpeed * 15;
    if (glfwGetKey(&window.getWindow(), GLFW_KEY_E) == GLFW_PRESS)
        camera_rot[0] += cameraSpeed * 15;
    if (glfwGetKey(&window.getWindow(), GLFW_KEY_UP) == GLFW_PRESS)
        camera_rot[2] += cameraSpeed * 15;
    if (glfwGetKey(&window.getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS)
        camera_rot[2] -= cameraSpeed * 15;
    if (glfwGetKey(&window.getWindow(), GLFW_KEY_LEFT) == GLFW_PRESS)
        camera_rot[1] += cameraSpeed * 15;
    if (glfwGetKey(&window.getWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera_rot[1] -= cameraSpeed * 15;

    if (glfwGetKey(&window.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
        camera_pos[0] += cameraSpeed;
    if (glfwGetKey(&window.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
        camera_pos[0] -= cameraSpeed;
    if (glfwGetKey(&window.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
        camera_pos[2] -= cameraSpeed;
    if (glfwGetKey(&window.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
        camera_pos[2] += cameraSpeed;

    if (glfwGetKey(&window.getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
        camera_pos[1] += cameraSpeed;
    if (glfwGetKey(&window.getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera_pos[1] -= cameraSpeed;
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
        ImGui::MenuItem("New", "Ctrl+N");
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
    {
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Settings"))
    {
       ImGui::MenuItem("Settings", nullptr, &show_settings_window);
       ImGui::MenuItem("Navigation", nullptr, &show_navigation_window);
       ImGui::MenuItem("Demo", nullptr, &show_demo);
       ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Help"))
    {
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();

    if (show_demo)
    {
        ImGui::ShowDemoWindow();
    }

    if (show_navigation_window)
    {
        // Settings Window
        ImGui::Begin("Navigation", &show_navigation_window, ImGuiChildFlags_AlwaysAutoResize);

        ImGui::SliderFloat3("Scale", scale, 0.f, 2.f);
        ImGui::SliderFloat("Rotate", &rotate, 0.f, 360.f);
        ImGui::SliderFloat3("Translate", translate, -100.f, 100.f);

        ImGui::Dummy(ImVec2(0, 30));

        ImGui::SliderFloat3("Camera position", camera_pos, -10.f, 10.f);
        ImGui::SliderFloat3("Camera rotation", camera_rot, 0, 360.f);

        ImGui::Dummy(ImVec2(0, 30));

        ImGui::Checkbox("Perspective camera", &perspective_camera);

        ImGui::End();
    }

    if (show_settings_window)
    {
        std::vector<smpl::VideoMode> m_available_modes;
        std::vector<std::string> m_mode_labels;

        if (!m_available_modes.empty())
            return;

        auto modes = smpl::VideoMode::getFullscreenModes();

        std::set<std::pair<int, int>> seen;
        std::vector<smpl::VideoMode> unique;

        for (const auto& mode : modes)
        {
            auto key = std::make_pair(mode.width, mode.height);
            if (seen.insert(key).second)
            {
                unique.push_back(mode);
            }
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
                {
                    m_selected_index = i;
                }
                if (selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
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
