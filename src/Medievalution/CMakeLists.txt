cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

project(MedievalutionApp)

add_executable(Medievalution3D
    src/main.cpp
)

target_include_directories(Medievalution3D PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(Medievalution3D PUBLIC cxx_std_20)
target_link_libraries(Medievalution3D PRIVATE Samopal_Engine)