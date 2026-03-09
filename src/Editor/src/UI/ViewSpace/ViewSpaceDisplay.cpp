#include "ViewSpaceDisplay.hpp"
#include <imgui.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Editor
{
    ViewSpaceDisplay::ViewSpaceDisplay()
        : m_framebuffer_texture(0)
        , m_depth_renderbuffer(0)
        , m_framebuffer(0)
        , m_current_width(640)
        , m_current_height(480)
    {
    }

    ViewSpaceDisplay::~ViewSpaceDisplay()
    {
        if (m_framebuffer_texture != 0)
        {
            glDeleteTextures(1, &m_framebuffer_texture);
        }
        if (m_depth_renderbuffer != 0)
        {
            glDeleteRenderbuffers(1, &m_depth_renderbuffer);
        }
        if (m_framebuffer != 0)
        {
            glDeleteFramebuffers(1, &m_framebuffer);
        }
    }

    void ViewSpaceDisplay::initializeFramebuffer()
    {
        // Create framebuffer
        glGenFramebuffers(1, &m_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

        // Create texture for color attachment
        glGenTextures(1, &m_framebuffer_texture);
        glBindTexture(GL_TEXTURE_2D, m_framebuffer_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_framebuffer_texture, 0);

        // Create renderbuffer for depth attachment
        glGenRenderbuffers(1, &m_depth_renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_depth_renderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1024, 768);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depth_renderbuffer);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            glDeleteFramebuffers(1, &m_framebuffer);
            glDeleteTextures(1, &m_framebuffer_texture);
            glDeleteRenderbuffers(1, &m_depth_renderbuffer);
            m_framebuffer = 0;
            m_framebuffer_texture = 0;
            m_depth_renderbuffer = 0;
        }
        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }

    void ViewSpaceDisplay::resizeFramebuffer(int width, int height)
    {
        if (width <= 0 || height <= 0) return;

        // Save current framebuffer binding
        GLint current_fbo;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_fbo);

        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

        // Resize texture
        glBindTexture(GL_TEXTURE_2D, m_framebuffer_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        // Resize renderbuffer
        glBindRenderbuffer(GL_RENDERBUFFER, m_depth_renderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

        // Restore previous framebuffer binding
        glBindFramebuffer(GL_FRAMEBUFFER, current_fbo);
    }

    void ViewSpaceDisplay::show()
    {
        if (m_show_scene)
        {
            ImGui::Begin("Scene");

            ImVec2 window_size = ImGui::GetContentRegionAvail();
            ImVec2 scene_pos = ImGui::GetCursorScreenPos();
            ImVec2 scene_size(window_size.x, window_size.y - 30);

            if (m_framebuffer == 0)
            {
                initializeFramebuffer();
                if (m_framebuffer != 0)
                {
                    resizeFramebuffer(static_cast<int>(scene_size.x), static_cast<int>(scene_size.y));
                    m_current_width = static_cast<int>(scene_size.x);
                    m_current_height = static_cast<int>(scene_size.y);
                }
            }
            else if (static_cast<int>(scene_size.x) != m_current_width ||
                static_cast<int>(scene_size.y) != m_current_height)
            {
                resizeFramebuffer(static_cast<int>(scene_size.x), static_cast<int>(scene_size.y));
                m_current_width = static_cast<int>(scene_size.x);
                m_current_height = static_cast<int>(scene_size.y);
            }

            if (m_framebuffer != 0)
            {
                GLint last_viewport[4];
                GLint last_framebuffer;
                glGetIntegerv(GL_VIEWPORT, last_viewport);
                glGetIntegerv(GL_FRAMEBUFFER_BINDING, &last_framebuffer);

                glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
                glViewport(0, 0, static_cast<GLsizei>(scene_size.x), static_cast<GLsizei>(scene_size.y));

                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glEnable(GL_DEPTH_TEST);

                //layer.show();

                glBindFramebuffer(GL_FRAMEBUFFER, last_framebuffer);
                glViewport(last_viewport[0], last_viewport[1], last_viewport[2], last_viewport[3]);

                ImGui::Image((void*)(uintptr_t)m_framebuffer_texture, scene_size, ImVec2(0, 1), ImVec2(1, 0));
            }
            else
            {
                ImDrawList* draw_list = ImGui::GetWindowDrawList();
                draw_list->AddRectFilled(scene_pos, ImVec2(scene_pos.x + scene_size.x, scene_pos.y + scene_size.y), IM_COL32(0, 0, 0, 255));
                draw_list->AddRect(scene_pos, ImVec2(scene_pos.x + scene_size.x, scene_pos.y + scene_size.y), IM_COL32(100, 100, 100, 255));
            }

            ImGui::Text("Scene Viewport: %.0fx%.0f", scene_size.x, scene_size.y);
            ImGui::End();
        }
    }
}
