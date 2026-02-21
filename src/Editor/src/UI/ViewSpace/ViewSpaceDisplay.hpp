#pragma once

#include <glad/glad.h>

namespace Editor
{
    class Scene;

    class ViewSpaceDisplay
    {
    public:
        ViewSpaceDisplay(Scene& scene);
        ~ViewSpaceDisplay();

        void show();

    private:
        Scene& m_scene;
        bool m_show_scene = true;

        unsigned int m_framebuffer;
        unsigned int m_framebuffer_texture;
        unsigned int m_depth_renderbuffer;
        int m_current_width = 1024;
        int m_current_height = 768;

        void initializeFramebuffer();
        void resizeFramebuffer(int width, int height);
    };
}
