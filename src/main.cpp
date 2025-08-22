#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Window/Window.h"
#include "Log.h"

GLfloat point[] = {
    0.0f,  0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
   -0.5f, -0.5f, 0.0f
};

GLfloat colors [] = {
    1.0f,  0.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
    0.0f,  0.0f, 1.0f
};

const char* vertex_shader = {
    "#version 460\n"
    "layout (location = 0 )in vec3 vertex_position;"
    "layout (location = 1 )in vec3 vertex_color;"
    "uniform float angle;"
    "out vec3 color;"
    "void main()"
    "{"
    "color = vertex_color;"
    "float c = cos(angle);"
    "float s = sin(angle);"
    "float y = c * vertex_position.y - s * vertex_position.z;"
    "float z = s * vertex_position.y + c * vertex_position.z;"
    "vec3 rotated_position = vec3(vertex_position.x, y, z);"
    "gl_Position = vec4(rotated_position, 1.0);"
    "}"
};

const char* fragment_shader = {
    "#version 460\n"
    "in vec3 color;"
    "out vec4 frag_color;"
    "void main()"
    "{"
    "frag_color = vec4(color, 1.0f);"
    "}"
};

bool initImGui(const Window& window)
{
    if (!window.window)
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

    if(!ImGui_ImplGlfw_InitForOpenGL(window.window, true))
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

    Window window;
    window.create(WIDTH, HEIGHT, "Test");

    initImGui(window);

    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig config;
    config.SizePixels = 25.0f;
    io.Fonts->AddFontDefault(&config);

#pragma region Shader

    GLuint v_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v_shader, 1, &vertex_shader, nullptr);

    GLuint f_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f_shader, 1, &fragment_shader, nullptr);

    GLuint shader = glCreateProgram();

    glAttachShader(shader, v_shader);
    glAttachShader(shader, f_shader);

    glLinkProgram(shader);

    glDeleteShader(v_shader);
    glDeleteShader(f_shader);

    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

    GLuint colors_vbo = 0;
    glGenBuffers(1, &colors_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    const GLint uniform_angle = glGetUniformLocation(shader, "angle");

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

#pragma endregion

    Color color{ 50,50,50 };

    float angle = 0.0f;

    while (!glfwWindowShouldClose(window.window))
    {
        window.clear(color);

        initBackEndImGui();

        glUseProgram(shader);
        glUniform1f(uniform_angle, angle);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        ImGui::NewFrame();

        //ImGui::ShowDemoWindow();
        ImGui::Begin("Transform");
        ImGui::SliderFloat ("Roataion", &angle, 0, 3.0f, "%.1f");
        ImGui::End();
        
        ImGui::Render();
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwPollEvents();
        
        window.display();
    }

    destroyImGui();

    window.close();
    return 0;
}
