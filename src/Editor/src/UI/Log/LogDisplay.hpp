#pragma once
#include <spdlog/sinks/base_sink.h>
#include <spdlog/details/null_mutex.h>
#include <vector>
#include <string>
#include <mutex>

struct ImVec4;

namespace Editor
{
    class LogDisplay
    {
    public:
        LogDisplay();
        ~LogDisplay() = default;

        void show();

    private:
        struct LogEntry
        {
            std::string message;
            spdlog::level::level_enum level{ spdlog::level::info };
            std::string timestamp;
        };

        class LogDisplaySink
            : public spdlog::sinks::base_sink<std::mutex>
        {
        public:
            LogDisplaySink() = default;

            std::vector<LogEntry> get_logs() const;
            void clear_logs();

        protected:
            void sink_it_(const spdlog::details::log_msg& msg) override;
            void flush_() override;

        private:
            std::vector<LogEntry> m_logs;
            mutable std::mutex m_logs_mutex;
            static constexpr size_t m_max_logs = 100;
        };

        void logging(const LogEntry& entry);
        ImVec4 getLevelColor(spdlog::level::level_enum level);
        const char* getLevelName(spdlog::level::level_enum level);

    private:
        std::shared_ptr<LogDisplaySink> log_sink;
        bool m_auto_scroll       = true;
        bool m_show_timestamps   = true;
        bool m_show_debug        = true;
        bool m_show_info         = true;
        bool m_show_warn         = true;
        bool m_show_error        = true;
        bool m_show_critical     = true;
        char m_search_query[256] = "";
    };
}