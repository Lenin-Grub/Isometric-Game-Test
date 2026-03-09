#pragma once

namespace smpl
{
    class NonCopyable
    {
    protected:
        NonCopyable()          = default;
        virtual ~NonCopyable() = default;

        NonCopyable(const NonCopyable&)            = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;
    };

    class NonMovable
    {
    protected:
        NonMovable()          = default;
        virtual ~NonMovable() = default;

        NonMovable(NonMovable&&)            = delete;
        NonMovable& operator=(NonMovable&&) = delete;
    };
}