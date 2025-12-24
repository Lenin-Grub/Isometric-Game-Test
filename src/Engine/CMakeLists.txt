cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

project(SamopalEngine)

include(${CMAKE_CURRENT_SOURCE_DIR}/engine.cmake)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Engine" FILES ${ENGINE_PUBLIC_INCLUDES} ${ENGINE_PUBLIC_SOURCES})

add_library(${PROJECT_NAME} STATIC
    ${ENGINE_PUBLIC_INCLUDES}
    ${ENGINE_PUBLIC_SOURCES}
)

set(EXTERNAL_DIR $ENV{External} CACHE PATH "Path to external dependencies")

add_subdirectory(${EXTERNAL_DIR}/glfw            External/glfw)
add_subdirectory(${EXTERNAL_DIR}/glad            External/glad)
add_subdirectory(${EXTERNAL_DIR}/glm             External/glm)
add_subdirectory(${EXTERNAL_DIR}/spdlog          External/spdlog)

#imgui
list(APPEND IMGUI_SOURCES
    ${EXTERNAL_DIR}/imgui/imgui.cpp
    ${EXTERNAL_DIR}/imgui/imgui_demo.cpp
    ${EXTERNAL_DIR}/imgui/imgui_draw.cpp
    ${EXTERNAL_DIR}/imgui/imgui_tables.cpp
    ${EXTERNAL_DIR}/imgui/imgui_widgets.cpp
    ${EXTERNAL_DIR}/imgui/backends/imgui_impl_glfw.cpp
    ${EXTERNAL_DIR}/imgui/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PRIVATE ${EXTERNAL_DIR}/imgui ${EXTERNAL_DIR}/imgui/backends)
target_compile_definitions(imgui PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_link_libraries     (imgui PRIVATE glfw glad)

#stb image
add_library(stb_image INTERFACE)
target_compile_definitions(stb_image INTERFACE STB_IMAGE_IMPLEMENTATION)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    
    ${EXTERNAL_DIR}/glfw/include
    ${EXTERNAL_DIR}/glad/include
    ${EXTERNAL_DIR}/spdlog/include
    ${EXTERNAL_DIR}/glm
    ${EXTERNAL_DIR}/imgui
    ${EXTERNAL_DIR}/imgui/backends
    ${CMAKE_SOURCE_DIR}/ext_libs
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glad
    glm::glm
    spdlog
    stb_image
    imgui
)

#target_precompile_headers(${PROJECT_NAME} PRIVATE src/stdafx.hpp)