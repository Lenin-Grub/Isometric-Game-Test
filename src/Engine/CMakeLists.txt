cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

project(Samopal_Engine)

set (ENGINE_PUBLIC_INCLUDES
    includes/Window/Window.hpp
    includes/Shader/Shader.hpp
    includes/Event/Event.hpp
    includes/Log/Log.hpp
)

set (ENGINE_PUBLIC_SOURCES
    src/Window/Window.cpp
    src/Shader/Shader.cpp
    src/Event/Event.cpp
)

add_library(${PROJECT_NAME} STATIC
	${ENGINE_PUBLIC_INCLUDES}
	${ENGINE_PUBLIC_SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/includes

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