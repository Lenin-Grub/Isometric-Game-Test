#include "Settings/Settings.hpp"

#include <fstream>
#include <string>
#include <Log/Log.hpp>

namespace smpl
{
    bool Settings::saveToFile(const std::string& path) const noexcept
    {
        try
        {
            std::filesystem::path full_path(path);
            std::filesystem::create_directories(full_path.parent_path());

            json j;
            j["window"]["title"]         = window.title;
            j["window"]["width"]         = window.mode.width;
            j["window"]["height"]        = window.mode.height;
            j["window"]["fullscreen"]    = window.fullscreen;
            j["window"]["vertical_sync"] = window.vertical_sync;
            j["window"]["resizable"]     = window.resizable;
            j["window"]["fps_limit"]     = window.fps_limit;

            j["audio"]["music_volume"]   = audio.music_volume;
            j["audio"]["sound_volume"]   = audio.sound_volume;
            j["audio"]["is_sound_play"]  = audio.is_sound_play;
            j["audio"]["is_music_play"]  = audio.is_music_play;

            j["language"]                = language;

            std::ofstream file(path);
            if (!file.is_open())
                return false;

            file << j.dump(4);
            return true;
        }
        catch (...)
        {
            LOG_CRITICAL("Settings not saved: {}", path);
            return false;
        }
    }

    bool Settings::loadFromFile(const std::string& path) noexcept
    {
        try
        {
            if (!std::filesystem::exists(path))
            {
                LOG_CRITICAL("Settings not found: {}", path);
                return false;
            }

            std::ifstream file(path);
            if (!file.is_open())
            {
                LOG_CRITICAL("Settings not open: {}", path);
                return false;
            }

            json j;
            file >> j;

            if (j.contains("window"))
            {
                auto& win            = j["window"];
                window.title         = win.value("title"        , "No Title");
                window.mode.width    = win.value("width"        , 680);
                window.mode.height   = win.value("height"       , 480);
                window.fullscreen    = win.value("fullscreen"   , false);
                window.vertical_sync = win.value("vertical_sync", false);
                window.resizable     = win.value("resizable"    , false);
                window.fps_limit     = win.value("fps_limit"    , 60);
            }

            if (j.contains("audio"))
            {
                auto& aud           = j["audio"];
                audio.music_volume  = aud.value("music_volume" , 0);
                audio.sound_volume  = aud.value("sound_volume" , 0);
                audio.is_sound_play = aud.value("is_sound_play", false);
                audio.is_music_play = aud.value("is_music_play", false);
            }

            language = j.value("language", "eng");
            return true;
        }
        catch (...)
        {
            LOG_CRITICAL("Settings not loaded: {}", path);
            return false;
        }
    }
}