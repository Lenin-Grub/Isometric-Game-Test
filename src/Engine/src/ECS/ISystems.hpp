#pragma once
#include "Components.hpp"

namespace smpl::ECS::Systems
{
    class ISystems
    {
    public:
        ISystems() = default;
        virtual ~ISystems() = default;

        virtual bool init() {};
        virtual void update() {};

        virtual void on() {};
        virtual void off() {};

        virtual void isActive() {};

    private:
    };
}
