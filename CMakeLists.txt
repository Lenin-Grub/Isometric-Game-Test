cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

project(Medievalution)

add_compile_definitions("DEBUG=$<CONFIG:Debug>")

add_subdirectory(src/Engine)
add_subdirectory(src/Editor)
add_subdirectory(src/Medievalution)
add_subdirectory(shaders)

set(GLFW_BUILD_DOCS      OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES  OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL         OFF CACHE BOOL "" FORCE)

set_target_properties(glfw            PROPERTIES FOLDER "External")
set_target_properties(update_mappings PROPERTIES FOLDER "External")
set_target_properties(glad            PROPERTIES FOLDER "External")
set_target_properties(imgui           PROPERTIES FOLDER "External")
set_target_properties(spdlog          PROPERTIES FOLDER "External")
set_target_properties(glm             PROPERTIES FOLDER "External")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Medievalution)