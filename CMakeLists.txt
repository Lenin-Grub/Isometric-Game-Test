cmake_minimum_required(VERSION 3.31 FATAL_ERROR)
project(Medievalution)

set(EXTERNAL_DIR $ENV{External})
if(NOT EXTERNAL_DIR OR NOT EXISTS ${EXTERNAL_DIR})
    set(EXTERNAL_DIR ${CMAKE_CURRENT_BINARY_DIR}/External)
    file(MAKE_DIRECTORY ${EXTERNAL_DIR})
    
    set(GLFW_URL "https://github.com/glfw/glfw.git")
    set(GLFW_TAG "3.4")
    set(GLM_URL "https://github.com/g-truc/glm.git")
    set(GLM_TAG "0.9.9.8")
    set(SPDLOG_URL "https://github.com/gabime/spdlog.git")
    set(SPDLOG_TAG "v1.13.0")
    set(IMGUI_URL "https://github.com/ocornut/imgui.git")
    set(IMGUI_TAG "docking")
    set(STB_URL "https://github.com/nothings/stb.git")
    set(STB_TAG "master")
    
    # Function to check if external project exists
    function(check_and_download_project NAME URL TAG SUBDIR)
        set(PROJECT_PATH ${EXTERNAL_DIR}/${SUBDIR})
        if(NOT EXISTS ${PROJECT_PATH})
            message(STATUS "Downloading ${NAME}...")
            execute_process(
                COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_PATH}
                WORKING_DIRECTORY ${EXTERNAL_DIR}
            )
            execute_process(
                COMMAND git clone --depth 1 --branch ${TAG} ${URL} ${SUBDIR}
                WORKING_DIRECTORY ${EXTERNAL_DIR}
                RESULT_VARIABLE GIT_RESULT
            )
            if(NOT GIT_RESULT EQUAL 0)
                message(FATAL_ERROR "Failed to clone ${NAME}")
            endif()
        endif()
    endfunction()
    
    check_and_download_project("GLFW"   ${GLFW_URL} ${GLFW_TAG}     "glfw")
    check_and_download_project("GLM"    ${GLM_URL} ${GLM_TAG}       "glm")
    check_and_download_project("SPDLOG" ${SPDLOG_URL} ${SPDLOG_TAG} "spdlog")
    check_and_download_project("IMGUI"  ${IMGUI_URL} ${IMGUI_TAG}   "imgui")
    check_and_download_project("STB"    ${STB_URL} ${STB_TAG}       "stb_image")
else()
    message(STATUS "Using external dependencies from: ${EXTERNAL_DIR}")
endif()

add_compile_definitions("DEBUG=$<CONFIG:Debug>")
add_subdirectory(src/Engine)
add_subdirectory(src/Editor)
add_subdirectory(src/Medievalution)
add_subdirectory(shaders)

set(GLFW_BUILD_DOCS      OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES  OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL         OFF CACHE BOOL "" FORCE)

set_target_properties(glfw            PROPERTIES FOLDER "External")
set_target_properties(update_mappings PROPERTIES FOLDER "External")
set_target_properties(glad            PROPERTIES FOLDER "External")
set_target_properties(imgui           PROPERTIES FOLDER "External")
set_target_properties(spdlog          PROPERTIES FOLDER "External")
set_target_properties(glm             PROPERTIES FOLDER "External")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Medievalution)
