cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(Medievalution)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/src/*.hpp ${PROJECT_SOURCE_DIR}/src/*.h ${PROJECT_SOURCE_DIR}/src/*.inl)
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_compile_definitions("DEBUG=$<CONFIG:Debug>")

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADERS} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)

set(BUILD_SHARED_LIBS OFF)

set(EXTERNAL_DIR $ENV{External} CACHE PATH "Path to external dependencies")

add_subdirectory(${EXTERNAL_DIR}/glfw    External/glfw)
add_subdirectory(${EXTERNAL_DIR}/glad    External/glad)
add_subdirectory(${EXTERNAL_DIR}/glm     External/glm)
add_subdirectory(${EXTERNAL_DIR}/spdlog  External/spdlog)


list(APPEND SOURCES
    ${EXTERNAL_DIR}/imgui/imgui.cpp
    ${EXTERNAL_DIR}/imgui/imgui_demo.cpp
    ${EXTERNAL_DIR}/imgui/imgui_draw.cpp
    ${EXTERNAL_DIR}/imgui/imgui_tables.cpp
    ${EXTERNAL_DIR}/imgui/imgui_widgets.cpp
    ${EXTERNAL_DIR}/imgui/backends/imgui_impl_glfw.cpp
    ${EXTERNAL_DIR}/imgui/backends/imgui_impl_opengl3.cpp
)

set_property(TARGET ${PROJECT_NAME} PROPERTY SOURCES ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${EXTERNAL_DIR}/glfw/include
    ${EXTERNAL_DIR}/glad/include
    ${EXTERNAL_DIR}/glm/include
    ${EXTERNAL_DIR}/spdlog/include
    ${EXTERNAL_DIR}/imgui
    ${EXTERNAL_DIR}/imgui/backends
)

target_compile_definitions(${PROJECT_NAME} PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        glfw
        glad
        glm::glm
        spdlog
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)