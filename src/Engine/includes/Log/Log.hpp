#pragma once
#include <spdlog/spdlog.h>

namespace LogInfo
{
    inline void initLogger()
    {
        //spdlog::set_level(spdlog::level::debug);
        spdlog::set_pattern("[%H:%M:%S] [%l] %v");
    }

#ifdef NDEBUG

#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#define LOG_CRITICAL(...)
#define LOG_DEBUG(...)

#else

#define LOG_INFO(...)     spdlog::info(__VA_ARGS__)
#define LOG_WARN(...)     spdlog::warn(__VA_ARGS__)
#define LOG_ERROR(...)    spdlog::error(__VA_ARGS__)
#define LOG_CRITICAL(...) spdlog::critical(__VA_ARGS__)
#define LOG_DEBUG(...)    spdlog::debug(__VA_ARGS__)

#endif
}