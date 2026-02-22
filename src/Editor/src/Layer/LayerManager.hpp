#pragma once

#include <Layer/ILayer.hpp>
#include <vector>
#include <memory>

namespace Editor 
{
    class LayerManager
    {
    public:
        ~LayerManager() = default;

        void addLayer(std::shared_ptr<Engine::ILayer> layer);
        void removeLayer(std::shared_ptr<Engine::ILayer> layer);

        void activateLayer(size_t index);
        void deactivateLayer(size_t index);

        void showLayer(size_t index);
        void hideLayer(size_t index);
        
        void initAll();
        void updateAll(float deltaTime);
        void renderAll();

        size_t getLayerCount() const;
        std::shared_ptr<Engine::ILayer> getLayer(size_t index);

    private:
        std::vector<std::shared_ptr<Engine::ILayer>> m_layers;
    };
}