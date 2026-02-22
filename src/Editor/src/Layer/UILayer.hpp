#pragma once

#include <Layer/ILayer.hpp>
#include <Window/Window.hpp>
#include "../UI/Display/Displays.hpp"

namespace Editor 
{
    class UILayer 
        : public Engine::ILayer
    {
    public:
        UILayer();
        virtual ~UILayer() = default;

        bool init() override;
        void update(float deltaTime)override;
        void render()override;

        void setWindow(smpl::Window* window) { m_window = window; }
        void setDisplays(Editor::Displays* displays) { m_displays = displays; }

    private:
        smpl::Window* m_window;
        Editor::Displays* m_displays;
    };
}
