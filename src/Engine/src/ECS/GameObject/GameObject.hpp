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

#include "GameObject.inl"
}
