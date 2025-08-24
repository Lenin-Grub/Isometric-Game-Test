cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

project(Samopal_Engine)

set (ENGINE_PUBLIC_INCLUDES
    src/Window/Window.hpp
    src/Event/Event.hpp
	
    src/Log/Log.hpp
	
    src/FileSystem/FileSystem.hpp
	
	src/Graphics/Shader/Shader.hpp
	src/Graphics/Sprite/Sprite.hpp
	src/Graphics/Texture/Texture.hpp
	src/Graphics/Color/Color.hpp
	src/Graphics/VertexArray/VertexArray.hpp
	src/Graphics/VertexBuffer/VertexBuffer.hpp
)

set (ENGINE_PUBLIC_SOURCES
    src/Window/Window.cpp
    src/Event/Event.cpp
	
	src/FileSystem/FileSystem.cpp
	
	src/Graphics/Shader/Shader.cpp
	src/Graphics/Sprite/Sprite.cpp
	src/Graphics/Texture/Texture.cpp
	src/Graphics/VertexArray/VertexArray.cpp
	src/Graphics/VertexBuffer/VertexBuffer.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Engine" FILES ${ENGINE_PUBLIC_INCLUDES} ${ENGINE_PUBLIC_SOURCES})

add_library(${PROJECT_NAME} STATIC
	${ENGINE_PUBLIC_INCLUDES}
	${ENGINE_PUBLIC_SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src

    $ENV{External}/glfw/include
    $ENV{External}/glad/include
    $ENV{External}/spdlog/include
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_link_libraries(${PROJECT_NAME} INTERFACE
    glfw
    glad
    glm::glm
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    spdlog
)