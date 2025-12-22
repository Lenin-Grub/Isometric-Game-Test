#pragma once
#include <Window/VideoMode.hpp>

#include <string>
#include <fstream>
#include <json/json.hpp>

using json = nlohmann::ordered_json;

namespace smpl
{
    struct WindowSettings
    {
        std::string     title         = "No Title";
        smpl::VideoMode mode          = { 680, 480 };
        bool            fullscreen    = false;
        bool            vertical_sync = false;
        bool            resizable     = true;
        std::uint32_t   fps_limit     = 60;
    };

    struct GraphicsSettings 
    {
        // FOV
        // Distance
        // Frustrum
        // etc.
    };

    struct AudioSettings
    {
        int  music_volume  = 0;
        int  sound_volume  = 0;
        bool is_sound_play = false;
        bool is_music_play = false;
    };

    struct Settings
    {
        WindowSettings   window;
        GraphicsSettings graphics;
        AudioSettings    audio;
        std::string      language = "eng";

        bool saveToFile(const std::string& path) const noexcept;
        bool loadFromFile(const std::string& path) noexcept;
    };
}