#include "Compass.hpp"

#include <cmath>
#include <numbers>
#include <vector>
#include <string>
#include <algorithm>
#include <Log/Log.hpp>


namespace Editor
{
    void Compass::show()
    {
        ImGui::Begin("Compass", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
        showCompass(m_angle);
        showCompass(m_angle);
        ImGui::End();
    }

    void Compass::setDirection(float angle)
    {
        m_angle = angle;
    }

    void Compass::showCompass(float value, const CompassStyle& style)
    {
        ImDrawList* draw_lists = ImGui::GetWindowDrawList();
        float window_width     = ImGui::GetWindowWidth();
        ImVec2 screen_pos      = ImGui::GetCursorScreenPos();
        ImVec2 pos             = ImVec2((screen_pos.x + window_width * 0.5) - (style.width * 0.5), screen_pos.y);
        const float half_fov   = style.fov_degrees * 0.5f;
        const float scale      = style.width / style.fov_degrees;
        const float center_x   = pos.x + style.width * 0.5f;

        draw_lists->AddRectFilled(pos, ImVec2(pos.x + style.width, pos.y + 32), style.back_color);
        draw_lists->AddLine(pos, ImVec2(pos.x + style.width, pos.y), style.tick_color, style.horizontal_thickness);

        value = fmodf(value, 360.0f);
        value = roundf(value / style.step_degrees) * style.step_degrees;
        if (value < 0.0f)
            value += 360.0f;
        if (value >= 360.0f)
            value -= 360.0f;

        // Labels
        std::vector<std::pair<float, std::string>> labels;
        int steps = static_cast<int>(std::ceil(style.fov_degrees / style.step_degrees)) + 1;

        for (int i = -steps / 2; i <= steps / 2; ++i)
        {
            float degrees = value + i * style.step_degrees;
            degrees = fmodf(degrees, 360.0f);

            if (degrees < 0.0f)
                degrees += 360.0f;

            std::string direction_label = "";
            bool is_letter = false;
            float r = fmodf(degrees, 45.0f);

            if (r < 0.1f || r > 44.9f)
            {
                is_letter = true;
                int nearest = static_cast<int>(roundf(degrees / 45.0f)) * 45;
                switch (nearest % 360)
                {
                case 0:   direction_label = "N";  break;
                case 45:  direction_label = "NW"; break;
                case 90:  direction_label = "W";  break;
                case 135: direction_label = "SW"; break;
                case 180: direction_label = "S";  break;
                case 225: direction_label = "SE"; break;
                case 270: direction_label = "E";  break;
                case 315: direction_label = "NE"; break;
                default:  direction_label = "";
                }
            }

            if (!is_letter)
            {
                char buffer[8];
                snprintf(buffer, sizeof(buffer), "%.0f", degrees);
                direction_label = buffer;
            }

            float delta = degrees - value;
            if (delta > 180.0f)
                delta -= 360.0f;
            if (delta < -180.0f)
                delta += 360.0f;
            if (fabsf(delta) <= half_fov)
                labels.emplace_back(degrees, direction_label);
        }


        // Duplicates on the corners
        std::sort(labels.begin(), labels.end(),
            [](const auto& a, const auto& b) { return a.first < b.first; });
        auto last = std::unique(labels.begin(), labels.end(),
            [](const auto& a, const auto& b) { return fabsf(a.first - b.first) < 0.5f; });
        labels.erase(last, labels.end());

        for (auto& [degrees, lbl] : labels)
        {
            float delta = degrees - value;
            if (delta > 180.0f)
                delta -= 360.0f;
            if (delta < -180.0f)
                delta += 360.0f;
            if (fabsf(delta) > half_fov)
                continue;

            float pos_x = center_x + delta * scale;

            // Sticks
            draw_lists->AddLine(ImVec2(pos_x, pos.y - style.tick_length * 0.5f), ImVec2(pos_x, pos.y + style.tick_length * 0.5f), style.tick_color, style.tick_thickness);

            // Text
            ImVec2 size = ImGui::CalcTextSize(lbl.c_str());
            float text_x = pos_x - size.x * 0.5f;
            float text_y = pos.y + style.tick_length + 2;

            if (text_x >= pos.x && text_x + size.x <= pos.x + style.width)
            {
                ImU32 col = (fabsf(fmodf(degrees, 45.0f)) < 0.1f || fabsf(fmodf(degrees, 45.0f) - 45.0f) < 0.1f) ? style.direction_color : IM_COL32(220, 220, 220, 255);
                draw_lists->AddText(ImVec2(text_x, text_y), col, lbl.c_str());
            }

            // Marker
            draw_lists->AddLine(ImVec2(center_x, pos.y - 10), ImVec2(center_x, pos.y + 10), style.marker_color, style.marker_thickness);
        }
    }
}