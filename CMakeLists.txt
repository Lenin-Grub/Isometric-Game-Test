cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

project(Medievalution)

add_compile_definitions("DEBUG=$<CONFIG:Debug>")

add_subdirectory(src/Engine)
add_subdirectory(src/Editor)
add_subdirectory(src/Medievalution)
add_subdirectory(shaders)

set_target_properties(glfw            PROPERTIES FOLDER "External/glfw")
set_target_properties(docs            PROPERTIES FOLDER "External/glfw")
set_target_properties(uninstall       PROPERTIES FOLDER "External/glfw")
set_target_properties(update_mappings PROPERTIES FOLDER "External/glfw")
set_target_properties(glad            PROPERTIES FOLDER "External")
set_target_properties(imgui           PROPERTIES FOLDER "External")
set_target_properties(spdlog          PROPERTIES FOLDER "External")
set_target_properties(glm             PROPERTIES FOLDER "External")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Medievalution)