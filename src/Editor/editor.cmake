set (EDITOR_PUBLIC_INCLUDES 
	src/Application/Application.hpp

    src/UI/Log/LogDisplay.hpp
    src/UI/Display/Displays.hpp
    src/UI/TabMenu/TabMenuDisplay.hpp
    src/UI/ViewSpace/ViewSpaceDisplay.hpp
    src/UI/SceneHierarchy/SceneHierarchy.hpp
    src/UI/ObjectDetails/ObjectDetailsDisplay.hpp
    src/UI/Tools/ToolsDisplay.hpp
    src/UI/Assets/AssetsDisplay.hpp
    src/UI/SettingsOverlay/SettingsOverlay.hpp

    src/Layer/BaseLayer.hpp
    src/Layer/UILayer.hpp
    src/Layer/LayerManager.hpp
)

set (EDITOR_PUBLIC_SOURCES 
    src/Application/Application.cpp

    src/UI/Log/LogDisplay.cpp
    src/UI/Display/Displays.cpp
    src/UI/TabMenu/TabMenuDisplay.cpp
    src/UI/ViewSpace/ViewSpaceDisplay.cpp
    src/UI/SceneHierarchy/SceneHierarchy.cpp
    src/UI/ObjectDetails/ObjectDetailsDisplay.cpp
    src/UI/Tools/ToolsDisplay.cpp
    src/UI/Assets/AssetsDisplay.cpp
    src/UI/SettingsOverlay/SettingsOverlay.cpp

    src/Layer/BaseLayer.cpp
    src/Layer/UILayer.cpp
    src/Layer/LayerManager.cpp

    src/main.cpp
)