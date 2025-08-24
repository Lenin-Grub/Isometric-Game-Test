cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

project(Medievalution)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Medievalution)

add_compile_definitions("DEBUG=$<CONFIG:Debug>")

set(EXTERNAL_DIR $ENV{External} CACHE PATH "Path to external dependencies")

add_subdirectory(${EXTERNAL_DIR}/glfw    External/glfw)
add_subdirectory(${EXTERNAL_DIR}/glad    External/glad)
add_subdirectory(${EXTERNAL_DIR}/glm     External/glm)
add_subdirectory(${EXTERNAL_DIR}/spdlog  External/spdlog)

add_subdirectory(src/Engine)
add_subdirectory(src/Medievalution)

list(APPEND IMGUI_SOURCES
    ${EXTERNAL_DIR}/imgui/imgui.cpp
    ${EXTERNAL_DIR}/imgui/imgui_demo.cpp
    ${EXTERNAL_DIR}/imgui/imgui_draw.cpp
    ${EXTERNAL_DIR}/imgui/imgui_tables.cpp
    ${EXTERNAL_DIR}/imgui/imgui_widgets.cpp
    ${EXTERNAL_DIR}/imgui/backends/imgui_impl_glfw.cpp
    ${EXTERNAL_DIR}/imgui/backends/imgui_impl_opengl3.cpp
)

#stb image
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/ext_libs)
target_compile_definitions(stb_image INTERFACE STB_IMAGE_IMPLEMENTATION)
target_link_libraries(Samopal_Engine PRIVATE stb_image)
#delete later
target_link_libraries(Medievalution PRIVATE stb_image)

#imgui
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PRIVATE ${EXTERNAL_DIR}/imgui ${EXTERNAL_DIR}/imgui/backends)
target_compile_definitions(imgui PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_link_libraries(imgui PRIVATE glfw glad)

target_link_libraries(Medievalution PRIVATE imgui)

target_include_directories(Medievalution PRIVATE
    ${EXTERNAL_DIR}/glfw/include
    ${EXTERNAL_DIR}/glad/include
    ${EXTERNAL_DIR}/glm/include
    ${EXTERNAL_DIR}/spdlog/include
    ${EXTERNAL_DIR}/imgui
    ${EXTERNAL_DIR}/imgui/backends
    src/Engine/src
    src/Medievalution/src
)

target_link_libraries(Medievalution PRIVATE
    Samopal_Engine
    glfw
    glad
    glm::glm
    spdlog
)

set_target_properties(Medievalution PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
)

target_compile_features(Medievalution PUBLIC cxx_std_20)