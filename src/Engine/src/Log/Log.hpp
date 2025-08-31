#pragma once

#pragma warning(push, 0)
#pragma warning(disable: 4996)
#include <spdlog/spdlog.h>
#pragma warning(pop)

namespace LogInfo
{
#ifdef DEBUG
    inline bool initLogger()
    {
        spdlog::set_level(spdlog::level::debug);
        return true;
    }
#endif

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