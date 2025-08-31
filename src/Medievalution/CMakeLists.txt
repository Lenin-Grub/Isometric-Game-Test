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
target_link_libraries(Medievalution PRIVATE SamopalEngine)

add_custom_command(TARGET Medievalution POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/shaders" "$<TARGET_FILE_DIR:Medievalution>/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/res"     "$<TARGET_FILE_DIR:Medievalution>/res"
)