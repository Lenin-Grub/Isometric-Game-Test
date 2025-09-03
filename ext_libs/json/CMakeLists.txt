cmake_minimum_required(VERSION 3.31)
project(json)

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} json.cpp)

target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR})