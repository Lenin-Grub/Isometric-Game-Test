#include "GameObject.hpp"
#include <Log/Log.hpp>

namespace smpl::ECS
{
    GameObject::GameObject(entt::registry& registry)
        : m_registry(registry)
    {
    }

    entt::entity GameObject::create()
    {
        if (m_entity != entt::null) 
        {
            LOG_WARN("GameObject already has an entity (ID: {}). Skipping creation.", getID());
            return m_entity;
        }

        m_entity = m_registry.create();
        LOG_INFO("Create Game Object: {}", getID());
        return m_entity;
    }

    void GameObject::remove()
    {
        if (m_entity != entt::null && m_registry.valid(m_entity))
        {
            LOG_INFO("Remove Game Object: {}", getID());
            m_registry.destroy(m_entity);
            m_entity = entt::null;
        }
    }

    const std::uint32_t GameObject::getID() const
    {
        return entt::to_integral(m_entity);
    }
}