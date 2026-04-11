#pragma once
#include "GameObject.hpp"

template <typename TComponent, typename... Args>
inline TComponent& smpl::ECS::GameObject::addComponent(Args&&... args)
{
    if (m_entity == entt::null)
    {
        throw std::runtime_error("GameObject has no valid entity.");
    }
    return m_registry.emplace<TComponent>(m_entity, std::forward<Args>(args)...);
}

template <typename TComponent>
inline void smpl::ECS::GameObject::removeComponent()
{
    if (m_entity == entt::null)
    {
        throw std::runtime_error("GameObject has no valid entity.");
    }
    m_registry.remove<TComponent>(m_entity);
}

template <typename TComponent, typename... Args>
inline TComponent& smpl::ECS::GameObject::replaceComponent(Args&&... args)
{
    if (m_entity == entt::null)
    {
        throw std::runtime_error("GameObject has no valid entity.");
    }
    if (m_registry.all_of<TComponent>(m_entity))
    {
        return m_registry.replace<TComponent>(m_entity, std::forward<Args>(args)...);
    }
    else {
        return m_registry.emplace<TComponent>(m_entity, std::forward<Args>(args)...);
    }
}

template <typename TComponent>
inline TComponent& smpl::ECS::GameObject::getComponent()
{
    if (!m_registry.all_of<TComponent>(m_entity))
    {
        throw std::runtime_error("GameObject does not have component.");
    }
    return m_registry.get<TComponent>(m_entity);
}

    template <typename TComponent>
    inline bool smpl::ECS::GameObject::hasComponent()
    {
        if (m_entity == entt::null)
        {
            return false;
        }
        return m_registry.all_of<TComponent>(m_entity);
    }