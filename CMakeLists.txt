cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(Isometric_Game)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

FILE(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/src/*.hpp ${PROJECT_SOURCE_DIR}/src/*.h ${PROJECT_SOURCE_DIR}/src/*.inl)
FILE(GLOB_RECURSE SOURCES  ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_compile_definitions("DEBUG=$<CONFIG:Debug>")

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}  FILES ${HEADERS} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)


set(BUILD_SHARED_LIBS OFF)
set(EXTERNAL_DIR $ENV{External})

add_subdirectory(${EXTERNAL_DIR}/glfw   External/glfw)
add_subdirectory(${EXTERNAL_DIR}/glad   External/glad)
add_subdirectory(${EXTERNAL_DIR}/glm    External/glm)
add_subdirectory(${EXTERNAL_DIR}/spdlog External/spdlog)
#add_subdirectory(${EXTERNAL_DIR}/glew   External/glew)
add_subdirectory(${EXTERNAL_DIR}/imgui  External/imgui)

target_include_directories(${PROJECT_NAME} PRIVATE 
	${EXTERNAL_DIR}/glfw/include
	${EXTERNAL_DIR}/glad/include
	${EXTERNAL_DIR}/glm/include
	${EXTERNAL_DIR}/spdlog/include
#	${EXTERNAL_DIR}/glew/include
	${EXTERNAL_DIR}/imgui/include
)


include_directories(${EXTERNAL_DIR}/glad/include)
include_directories(${imgui_SOURCE_DIR})
link_directories(${EXTERNAL_DIR}/glad/lib)

target_link_libraries(${PROJECT_NAME}
	glfw
	glad
	glm
	spdlog
#	glew
	imgui
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)