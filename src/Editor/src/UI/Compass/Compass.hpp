#pragma once
#include <imgui.h>

namespace Editor
{
    struct CompassStyle
    {
        float width                = 600.0f;
        float fov_degrees          = 120.0f;
        float step_degrees         = 10.0f;
        float tick_length          = 15.0f;
        float tick_thickness       = 3.0f;
        float marker_thickness     = 4.0f;
        float horizontal_thickness = 4.0f;

        ImU32 back_color           = IM_COL32(20, 20, 20, 0);
        ImU32 grid_color           = IM_COL32(30, 30, 30, 200);
        ImU32 tick_color           = IM_COL32(120, 120, 120, 255);
        ImU32 direction_color      = IM_COL32(255, 220, 51, 255);
        ImU32 marker_color         = IM_COL32(255, 255, 255, 255);
    };

    class Compass
    {
    public:
        void show();
        void setDirection(float angle);

    private:
        void showCompass(float value = 0.0f, const CompassStyle& style = {});

    private:
        float m_angle = 0;
    };
}
