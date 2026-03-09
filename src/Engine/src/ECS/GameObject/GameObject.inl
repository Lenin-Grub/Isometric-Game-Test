#pragma once

#include "GameObject.hpp"

namespace smpl::ECS
{
    template <typename TComponent, typename... Args>
    inline TComponent& GameObject::addComponent(Args&&... args)
    {
        if (m_entity == entt::null)
        {
            throw std::runtime_error("GameObject has no valid entity.");
        }
        return m_registry.emplace<TComponent>(m_entity, std::forward<Args>(args)...);
    }

    template <typename TComponent>
    inline void GameObject::removeComponent()
    {
        if (m_entity == entt::null)
        {
            throw std::runtime_error("GameObject has no valid entity.");
        }
        m_registry.remove<TComponent>(m_entity);
    }

    template <typename TComponent, typename... Args>
    inline TComponent& GameObject::replaceComponent(Args&&... args)
    {
        if (m_entity == entt::null)
        {
            throw std::runtime_error("GameObject has no valid entity.");
        }
        if (m_registry.all_of<TComponent>(m_entity))
        {
            return m_registry.replace<TComponent>(m_entity, std::forward<Args>(args)...);
        }
        else 
        {
            return m_registry.emplace<TComponent>(m_entity, std::forward<Args>(args)...);
        }
    }

    template <typename TComponent>
    inline TComponent& GameObject::getComponent()
    {
        if (!m_registry.all_of<TComponent>(m_entity))
        {
            throw std::runtime_error("GameObject does not have component.");
        }
        return m_registry.get<TComponent>(m_entity);
    }

    template <typename TComponent>
    inline bool GameObject::hasComponent()
    {
        if (m_entity == entt::null)
        {
            return false;
        }
        return m_registry.all_of<TComponent>(m_entity);
    }
}