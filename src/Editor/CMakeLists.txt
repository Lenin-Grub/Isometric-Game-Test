cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

project(SamopalEditor)

include(${CMAKE_CURRENT_SOURCE_DIR}/editor.cmake)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Editor" FILES ${EDITOR_PUBLIC_INCLUDES} ${EDITOR_PUBLIC_SOURCES})

add_executable(${PROJECT_NAME}
${EDITOR_PUBLIC_INCLUDES}
${EDITOR_PUBLIC_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE SamopalEngine)

add_custom_command(TARGET SamopalEditor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/shaders" "$<TARGET_FILE_DIR:Medievalution>/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/res"     "$<TARGET_FILE_DIR:Medievalution>/res"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/config"  "$<TARGET_FILE_DIR:Medievalution>/config"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
RUNTIME_OUTPUT_DIRECTORY         ${CMAKE_SOURCE_DIR}/bin
RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_SOURCE_DIR}/bin/Debug
RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release
VS_DEBUGGER_WORKING_DIRECTORY    ${CMAKE_SOURCE_DIR}/bin/Debug
)