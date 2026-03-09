#pragma once

#include <glad/glad.h>

namespace Editor
{
    class Scene;

    class ViewSpaceDisplay
    {
    public:
        ViewSpaceDisplay();
        ~ViewSpaceDisplay();

        void show();

    private:
        bool m_show_scene = true;

        unsigned int m_framebuffer;
        unsigned int m_framebuffer_texture;
        unsigned int m_depth_renderbuffer;
        int m_current_width;
        int m_current_height;

        void initializeFramebuffer();
        void resizeFramebuffer(int width, int height);
    };
}
