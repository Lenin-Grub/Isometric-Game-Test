cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

project(Medievalution)

set(GAME_INCLUDES
    src/Game/Game.hpp
)

set(GAME_SOURCES
    src/main.cpp
    
    src/Game/Game.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Medievalution" FILES ${GAME_INCLUDES} ${GAME_SOURCES})

add_executable(Medievalution
    ${GAME_INCLUDES}
    ${GAME_SOURCES}
)

target_include_directories(Medievalution PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(Medievalution PUBLIC cxx_std_20)

target_link_libraries(${PROJECT_NAME} PRIVATE SamopalEngine)
add_dependencies(${PROJECT_NAME} SamopalEngine)

add_custom_command(TARGET Medievalution POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/shaders" "$<TARGET_FILE_DIR:Medievalution>/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/res"     "$<TARGET_FILE_DIR:Medievalution>/res"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY         ${CMAKE_SOURCE_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_SOURCE_DIR}/bin/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release
    VS_DEBUGGER_WORKING_DIRECTORY    ${CMAKE_SOURCE_DIR}/bin/Debug
)