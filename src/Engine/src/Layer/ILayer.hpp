#pragma once

namespace Engine 
{
    class ILayer 
    {
    public:
        virtual ~ILayer() = default;

        virtual void activate() { m_isActive = true; };

        virtual void deactivate() { m_isActive = false; };

        virtual void show() { m_isVisible = true; };

        virtual void hide() { m_isVisible = false; };

        virtual bool isActive() { return m_isActive; };

        virtual bool isVisible() { return m_isVisible; };

        virtual bool init() { return false; };

        virtual void update(float deltaTime) {};

        virtual void render() {};

    private:
        bool m_isActive  = false;
        bool m_isVisible = true;
    };
}