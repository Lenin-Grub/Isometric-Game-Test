#include "LogDisplay.hpp"
#include <imgui.h>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>
//#include <Common/IconText.h>
#include <chrono>
#include <ctime>

namespace Editor
{
    std::vector<LogDisplay::LogEntry> LogDisplay::LogDisplaySink::get_logs() const
    {
        std::lock_guard<std::mutex> lock(m_logs_mutex);
        return m_logs;
    }

    void LogDisplay::LogDisplaySink::clear_logs()
    {
        std::lock_guard<std::mutex> lock(m_logs_mutex);
        m_logs.clear();
    }

    void LogDisplay::LogDisplaySink::sink_it_(const spdlog::details::log_msg& msg)
    {
        LogEntry entry;
        entry.message = fmt::to_string(msg.payload);
        entry.level = msg.level;

        auto time_t = std::chrono::system_clock::to_time_t(msg.time);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(msg.time.time_since_epoch()) % 1000;

        std::tm tm;
#if defined(_WIN32)
        localtime_s(&tm, &time_t);
#elif defined(__linux__)
        localtime_r(&time_t, &tm);
#endif

        char timestamp_buffer[64];
        std::strftime(timestamp_buffer, sizeof(timestamp_buffer), "%H:%M:%S", &tm);
        entry.timestamp = fmt::format("{}.{:03d}", timestamp_buffer, ms.count());

        {
            std::lock_guard<std::mutex> lock(m_logs_mutex);
            m_logs.push_back(entry);

            if (m_logs.size() > m_max_logs)
                m_logs.erase(m_logs.begin());
        }
    }

    void LogDisplay::LogDisplaySink::flush_()
    {
        // do nothing
    }

    LogDisplay::LogDisplay()
    {
        log_sink = std::make_shared<LogDisplaySink>();

        auto logger = spdlog::default_logger();
        logger->sinks().push_back(log_sink);
    }

    void LogDisplay::show()
    {
        ImGui::Begin("Logs");

        if (ImGui::Button("Settings"))
        {
            ImGui::OpenPopup("SettingsPopup");
        }

        if (ImGui::BeginPopup("SettingsPopup"))
        {
            ImGui::SeparatorText("Log Levels:");
            ImGui::Checkbox("Debug", &m_show_debug);
            ImGui::Checkbox("Info", &m_show_info);
            ImGui::Checkbox("Warning", &m_show_warn);
            ImGui::Checkbox("Error", &m_show_error);
            ImGui::Checkbox("Critical", &m_show_critical);

            ImGui::SeparatorText("Others:");
            ImGui::Checkbox("Auto-scroll", &m_auto_scroll);
            ImGui::Checkbox("Show timestamps", &m_show_timestamps);

            ImGui::EndPopup();
        }

        ImGui::SameLine();
        if (ImGui::Button("Clear"))
        {
            log_sink->clear_logs();
        }

        ImGui::SameLine();
        ImGui::SetNextItemWidth(300.0f);
        ImGui::InputText("Search", m_search_query, sizeof(m_search_query));

        ImGui::Separator();

        if (ImGui::BeginChild("LogArea", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar))
        {
            auto logs = log_sink->get_logs();

            for (const auto& entry : logs)
            {
                bool should_show = false;
                switch (entry.level)
                {
                case spdlog::level::debug:
                    should_show = m_show_debug;
                    break;
                case spdlog::level::info:
                    should_show = m_show_info;
                    break;
                case spdlog::level::warn:
                    should_show = m_show_warn;
                    break;
                case spdlog::level::err:
                    should_show = m_show_error;
                    break;
                case spdlog::level::critical:
                    should_show = m_show_critical;
                    break;
                default:
                    should_show = true;
                    break;
                }

                if (!should_show)
                    continue;

                if (m_search_query[0] != '\0' && entry.message.find(m_search_query) == std::string::npos)
                    continue;

                logging(entry);
            }

            if (m_auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            {
                ImGui::SetScrollHereY(1.0f);
            }
        }
        ImGui::EndChild();

        ImGui::End();
    }

    void LogDisplay::logging(const LogEntry& entry)
    {
        ImVec4 color = getLevelColor(entry.level);

        ImGui::PushID(static_cast<int>(std::hash<std::string>{}(entry.timestamp + entry.message) & 0x7FFFFFFF));

        ImGui::BeginGroup();

        if (m_show_timestamps)
        {
            ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "[%s]", entry.timestamp.c_str());
            ImGui::SameLine();
        }

        ImGui::TextColored(color, "[%s]", getLevelName(entry.level));
        ImGui::SameLine();
        ImGui::TextWrapped("%s", entry.message.c_str());

        ImGui::EndGroup();

        if (ImGui::BeginPopupContextItem("LogEntryContextMenu"))
        {
            if (ImGui::Selectable("Copy Line"))
            {
                std::string full_line;
                if (m_show_timestamps)
                {
                    full_line = fmt::format("[{}] [{}] {}", entry.timestamp, getLevelName(entry.level), entry.message);
                }
                else
                {
                    full_line = fmt::format("[{}] {}", getLevelName(entry.level), entry.message);
                }
                ImGui::SetClipboardText(full_line.c_str());
            }

            if (ImGui::Selectable("Copy All"))
            {
                std::string all_logs;
                auto logs = log_sink->get_logs();

                for (const auto& log_entry : logs)
                {
                    bool should_show = false;
                    switch (log_entry.level)
                    {
                    case spdlog::level::debug:
                        should_show = m_show_debug;
                        break;
                    case spdlog::level::info:
                        should_show = m_show_info;
                        break;
                    case spdlog::level::warn:
                        should_show = m_show_warn;
                        break;
                    case spdlog::level::err:
                        should_show = m_show_error;
                        break;
                    case spdlog::level::critical:
                        should_show = m_show_critical;
                        break;
                    default:
                        should_show = true;
                        break;
                    }

                    if (!should_show)
                        continue;

                    if (m_search_query[0] != '\0' && log_entry.message.find(m_search_query) == std::string::npos)
                        continue;

                    if (m_show_timestamps)
                    {
                        all_logs += fmt::format("[{}] [{}] {}\n", log_entry.timestamp, getLevelName(log_entry.level), log_entry.message);
                    }
                    else
                    {
                        all_logs += fmt::format("[{}] {}\n", getLevelName(log_entry.level), log_entry.message);
                    }
                }

                if (!all_logs.empty() && all_logs.back() == '\n')
                {
                    all_logs.pop_back();
                }

                ImGui::SetClipboardText(all_logs.c_str());
            }

            ImGui::EndPopup();
        }

        ImGui::PopID();
    }

    ImVec4 LogDisplay::getLevelColor(spdlog::level::level_enum level)
    {
        switch (level)
        {
        case spdlog::level::debug:
            return ImVec4(0.7f, 0.7f, 0.7f, 1.0f); // Gray
        case spdlog::level::info:
            return ImVec4(0.5f, 1.0f, 0.5f, 1.0f); // Green
        case spdlog::level::warn:
            return ImVec4(1.0f, 1.0f, 0.5f, 1.0f); // Yellow
        case spdlog::level::err:
            return ImVec4(1.0f, 0.5f, 0.5f, 1.0f); // Red
        case spdlog::level::critical:
            return ImVec4(1.0f, 0.2f, 0.2f, 1.0f); // Light Red
        default:
            return ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // White
        }
    }

    const char* LogDisplay::getLevelName(spdlog::level::level_enum level)
    {
        switch (level)
        {
        case spdlog::level::debug:    return "DEBUG";
        case spdlog::level::info:     return "INFO";
        case spdlog::level::warn:     return "WARN";
        case spdlog::level::err:      return "ERROR";
        case spdlog::level::critical: return "CRITICAL";
        default:                      return "UNKNOWN";
        }
    }
}