#pragma once
#include <entt.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/quaternion_float.hpp>


namespace smpl::ECS::Components
{
    struct Hierarchy
    {
        entt::entity parent = entt::null;
        // id
        // name
        // group
    };

    struct Transform
    {
        glm::vec3 position{ 0.0f };
        glm::vec3 rotation{ 0.0f };
        //glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
        glm::vec3 scale   { 1.0f };
    };

    struct Velocity
    {
        glm::vec3 value { 0.0f };
    };

    struct Color
    {
        glm::vec4 value{ 1.0f };
    };
}
