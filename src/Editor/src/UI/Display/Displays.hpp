#pragma once

#include "../Log/LogDisplay.hpp"
#include "../TabMenu/TabMenuDisplay.hpp"
#include "../ViewSpace/ViewSpaceDisplay.hpp"
#include "../ObjectDetails/ObjectDetailsDisplay.hpp"
#include "../SceneHierarchy/SceneHierarchy.hpp"
#include "../Tools/ToolsDisplay.hpp"
#include "../Assets/AssetsDisplay.hpp"

namespace Editor
{
    class Scene;

    class Displays
    {
    public:
        Displays(Editor::Scene& scene);
        ~Displays() = default;

        bool initDockSpace();
        bool create();
        void show();

        void initDockingLayout();

    private:
        Editor::LogDisplay               m_display_log;
        Editor::TabMenuDisplay           m_tab_menu;
        Editor::ViewSpaceDisplay         m_view_space;
        Editor::ObjectDetailsDisplay     m_object_details;
        Editor::SceneHierarchy           m_scene_hierarchy;
        Editor::AssetsDisplay            m_assets_display;
        Editor::ToolsDisplay             m_tools_display;
        bool m_docking_layout_initialized;
    };
}