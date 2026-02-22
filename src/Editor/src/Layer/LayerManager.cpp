#include "LayerManager.hpp"
#include <iostream>
#include <stdexcept>
#include <algorithm>

namespace Editor
{
    void LayerManager::addLayer(std::shared_ptr<Engine::ILayer> layer) 
    {
        if (!layer) 
        {
            throw std::invalid_argument("Cannot add null layer");
        }
        m_layers.push_back(layer);
    }

    void LayerManager::removeLayer(std::shared_ptr<Engine::ILayer> layer) 
    {
        if (!layer)
        {
            return;
        }

        auto it = std::find(m_layers.begin(), m_layers.end(), layer);
        if (it != m_layers.end())
        {
            m_layers.erase(it);
        }
    }

    void LayerManager::activateLayer(size_t index) 
    {
        if (index >= m_layers.size()) 
        {
            throw std::out_of_range("Layer index out of range");
        }
        m_layers[index]->activate();
    }

    void LayerManager::deactivateLayer(size_t index)
    {
        if (index >= m_layers.size())
        {
            throw std::out_of_range("Layer index out of range");
        }
        m_layers[index]->deactivate();
    }

    void LayerManager::showLayer(size_t index)
    {
        if (index >= m_layers.size()) 
        {
            throw std::out_of_range("Layer index out of range");
        }
        m_layers[index]->show();
    }

    void LayerManager::hideLayer(size_t index)
    {
        if (index >= m_layers.size()) 
        {
            throw std::out_of_range("Layer index out of range");
        }
        m_layers[index]->hide();
    }

    void LayerManager::initAll()
    {
        for (auto& layer : m_layers)
        {
            if (layer->isVisible())
            {
                layer->init();
            }
        }
    }

    void LayerManager::updateAll(float deltaTime) 
    {
        for (auto& layer : m_layers)
        {
            if (layer->isActive() && layer->isVisible()) 
            {
                layer->update(deltaTime);
            }
        }
    }

    void LayerManager::renderAll()
    {
        for (auto& layer : m_layers) 
        {
            if (layer->isVisible())
            {
                layer->render();
            }
        }
    }

    size_t LayerManager::getLayerCount() const {
        return m_layers.size();
    }

    std::shared_ptr<Engine::ILayer> LayerManager::getLayer(size_t index) {
        if (index >= m_layers.size()) {
            return nullptr;
        }
        return m_layers[index];
    }

}