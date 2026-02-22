#include "LayerManager.hpp"
#include "BaseLayer.hpp"
#include "UILayer.hpp"
#include <iostream>
#include <memory>

int main() {
    std::cout << "=== Layer System Test ===" << std::endl;

    // Create layer manager
    Editor::LayerManager layerManager;

    // Create layers
    auto baseLayer = std::make_shared<Editor::BaseLayer>();
    auto uiLayer = std::make_shared<Editor::UILayer>();

    // Add layers to manager
    layerManager.AddLayer(baseLayer);
    layerManager.AddLayer(uiLayer);

    std::cout << "\nInitial state:" << std::endl;
    std::cout << "BaseLayer active: " << baseLayer->IsActive() << ", visible: " << baseLayer->IsVisible() << std::endl;
    std::cout << "UILayer active: " << uiLayer->IsActive() << ", visible: " << uiLayer->IsVisible() << std::endl;

    // Activate both layers
    layerManager.ActivateLayer(0); // BaseLayer
    layerManager.ActivateLayer(1); // UILayer

    std::cout << "\nAfter activation:" << std::endl;
    std::cout << "BaseLayer active: " << baseLayer->IsActive() << ", visible: " << baseLayer->IsVisible() << std::endl;
    std::cout << "UILayer active: " << uiLayer->IsActive() << ", visible: " << uiLayer->IsVisible() << std::endl;

    // Test update and render
    std::cout << "\nTesting update and render:" << std::endl;
    layerManager.UpdateAll(0.016f); // ~60 FPS
    layerManager.RenderAll();

    // Hide UILayer
    layerManager.HideLayer(1);
    std::cout << "\nAfter hiding UILayer:" << std::endl;
    std::cout << "UILayer visible: " << uiLayer->IsVisible() << std::endl;

    // Test update and render with hidden UILayer
    std::cout << "\nTesting with hidden UILayer:" << std::endl;
    layerManager.UpdateAll(0.016f);
    layerManager.RenderAll();

    // Deactivate BaseLayer
    layerManager.DeactivateLayer(0);
    std::cout << "\nAfter deactivating BaseLayer:" << std::endl;
    std::cout << "BaseLayer active: " << baseLayer->IsActive() << std::endl;

    // Test update and render with deactivated BaseLayer
    std::cout << "\nTesting with deactivated BaseLayer:" << std::endl;
    layerManager.UpdateAll(0.016f);
    layerManager.RenderAll();

    std::cout << "\n=== Test Complete ===" << std::endl;
    return 0;
}