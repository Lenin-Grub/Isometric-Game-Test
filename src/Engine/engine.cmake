set (ENGINE_PUBLIC_INCLUDES
	src/Utils/NonCopyable.hpp
    src/Window/Window.hpp
    src/Window/VideoMode.hpp

    src/Event/Keyboard.hpp
    src/Event/Mouse.hpp
    src/Event/Input.hpp
    src/Event/Event.hpp
    src/Event/InputEvents.hpp
    src/Event/WindowEvents.hpp

    src/Log/Log.hpp
    src/ImGui/SetImGui.cpp

    src/Camera/Camera.hpp
    src/Settings/Settings.hpp	
    src/FileSystem/FileSystem.hpp
    
    src/Graphics/Shader/Shader.hpp
    src/Graphics/Sprite/Sprite.hpp
    src/Graphics/Texture/Texture.hpp
    src/Graphics/Color/Color.hpp
    src/Graphics/VertexArray/VertexArray.hpp
    src/Graphics/VertexBuffer/VertexBuffer.hpp
    src/Graphics/IndexBuffer/IndexBuffer.hpp
    
    src/Graphics/Primitives/Cube/Cube.hpp
    src/Graphics/Primitives/Grid/Grid.hpp

    src/Layer/ILayer.hpp

    src/ECS/ISystems.hpp
    src/ECS/Components.hpp
    src/ECS/GameObject/GameObject.hpp
    src/ECS/GameObject/GameObject.inl
    src/ECS/SceneManager/SceneManager.hpp
)

set (ENGINE_PUBLIC_SOURCES
    src/Window/Window.cpp
    src/Window/VideoMode.cpp

    src/Event/Keyboard.cpp
    src/Event/Mouse.cpp
    src/Event/Input.cpp

    src/ImGui/SetImGui.hpp
    src/Camera/Camera.cpp
    src/Settings/Settings.cpp
    src/FileSystem/FileSystem.cpp
    
    src/Graphics/Shader/Shader.cpp
    src/Graphics/Sprite/Sprite.cpp
    src/Graphics/Texture/Texture.cpp
    src/Graphics/VertexArray/VertexArray.cpp
    src/Graphics/VertexBuffer/VertexBuffer.cpp
    src/Graphics/IndexBuffer/IndexBuffer.cpp
    
    src/Graphics/Primitives/Cube/Cube.cpp
    src/Graphics/Primitives/Grid/Grid.cpp

    src/ECS/GameObject/GameObject.cpp
    src/ECS/SceneManager/SceneManager.cpp
)