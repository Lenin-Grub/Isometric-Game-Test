#pragma once
#include "../Components.hpp"

namespace smpl::ECS
{
    class GameObject
    {
    public:
        GameObject(entt::registry& registry);
        virtual ~GameObject() = default;

        entt::entity create();
        void remove();
        const std::uint32_t getID() const;

        entt::registry& getRegistry()
        {
            return m_registry;
        }

        // virtual void addParent() {};
        // virtual void removeParent() {};
        // virtual void getParent() {};
        // virtual void findParent() {};

        // virtual void addChild() {};
        // virtual void removeChild() {};
        // virtual void getChild() {};
        // virtual void findChild() {};
        // virtual void findAllChildrens() {};

        template <typename TComponent, typename... Args>
        TComponent& addComponent(Args&&... args);

        template <typename TComponent>
        void removeComponent();

        template <typename TComponent, typename... Args>
        TComponent& replaceComponent(Args&&... args);

        template <typename TComponent>
        TComponent& getComponent();

        template <typename TComponent>
        bool hasComponent();


    private:
        entt::registry& m_registry;
        entt::entity    m_entity  = entt::null;
    };

//#include "GameObject.inl"

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
        else {
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
