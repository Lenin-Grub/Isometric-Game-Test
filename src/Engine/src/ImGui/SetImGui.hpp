#pragma once

namespace smpl
{
    class Window;

    namespace Gui
    {
        bool initImGui(const smpl::Window& window);
        bool destroyImGui();
        bool initBackEndImGui();
        bool drawImGuiGL();
        bool initImGuiFont();
    }
}