cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

project(Medievalution)

add_executable(Medievalution
    src/main.cpp
)

target_include_directories(Medievalution PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(Medievalution PUBLIC cxx_std_20)
target_link_libraries(Medievalution PRIVATE Samopal_Engine)

add_custom_command(TARGET Medievalution POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/shaders" "$<TARGET_FILE_DIR:Medievalution>/shaders"
)