cmake_minimum_required(VERSION 3.28)

project(SpaceGame LANGUAGES CXX)

set(FETCHCONTENT_QUIET FALSE)
set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_WIDGETBUILDER "${CMAKE_CXX_FLAGS_DEBUG} -DWIDGETBUILDER=1")
set(CMAKE_C_FLAGS_WIDGETBUILDER "${CMAKE_C_FLAGS_DEBUG} -DWIDGETBUILDER=1")
set(CMAKE_EXE_LINKER_FLAGS_WIDGETBUILDER "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -DWIDGETBUILDER=1")
set(CMAKE_SHARED_LINKER_FLAGS_WIDGETBUILDER "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} -DWIDGETBUILDER=1")

set(ADDITIONAL_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Tools/CMake")

include(${ADDITIONAL_INCLUDE_PATH}/ServerConfig.cmake)
include(${ADDITIONAL_INCLUDE_PATH}/FetchContent.cmake)
include(${ADDITIONAL_INCLUDE_PATH}/Imgui.cmake)

set(SDL_LIBRARIES ${SDL_LIBRARIES} SDL2main SDL2 SDL2_image SDL2_ttf)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/Source")
file(GLOB_RECURSE src "${SRC_DIR}/*.h" "${SRC_DIR}/*.cpp")

add_executable(${PROJECT_NAME} ${src})

target_link_libraries("${PROJECT_NAME}" PRIVATE ${SDL_LIBRARIES} nlohmann_json::nlohmann_json imgui_lib)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<$<CONFIG:Debug>:${CMAKE_BINARY_DIR}/_deps/sdl2-build/Debug/SDL2d.dll>
    $<$<CONFIG:Debug>:${CMAKE_BINARY_DIR}/Debug>
    $<$<CONFIG:MinSizeRel>:${CMAKE_BINARY_DIR}/_deps/sdl2-build/MinSizeRel/SDL2.dll>
    $<$<CONFIG:MinSizeRel>:${CMAKE_BINARY_DIR}/MinSizeRel>
    )

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<$<CONFIG:Debug>:${CMAKE_BINARY_DIR}/_deps/sdl2Image-build/Debug/SDL2_imaged.dll>
    $<$<CONFIG:Debug>:${CMAKE_BINARY_DIR}/Debug>
    $<$<CONFIG:MinSizeRel>:${CMAKE_BINARY_DIR}/_deps/sdl2Image-build/MinSizeRel/SDL2_image.dll>
    $<$<CONFIG:MinSizeRel>:${CMAKE_BINARY_DIR}/MinSizeRel>
    )

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<$<CONFIG:Debug>:${CMAKE_BINARY_DIR}/_deps/sdl2ttf-build/Debug/SDL2_ttfd.dll>
    $<$<CONFIG:Debug>:${CMAKE_BINARY_DIR}/Debug>
    $<$<CONFIG:MinSizeRel>:${CMAKE_BINARY_DIR}/_deps/sdl2ttf-build/MinSizeRel/SDL2_ttf.dll>
    $<$<CONFIG:MinSizeRel>:${CMAKE_BINARY_DIR}/MinSizeRel>
    )

include(${CMAKE_CURRENT_SOURCE_DIR}/Tools/CMake/GroupSourcesByFolder.cmake)
GroupSourcesByFolder(${PROJECT_NAME})
