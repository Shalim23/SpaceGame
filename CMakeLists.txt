cmake_minimum_required(VERSION 3.28)

project(SpaceGame LANGUAGES CXX)

set(FETCHCONTENT_QUIET FALSE)
set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_CURRENT_SOURCE_DIR}/Tools/CMake/ServerConfig.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/Tools/CMake/FetchContent.cmake)

set(SDL_LIBRARIES ${SDL_LIBRARIES} SDL2main SDL2 SDL2_image)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}")
file(GLOB_RECURSE src "${SRC_DIR}/*.h" "${SRC_DIR}/*.cpp")

add_executable(${PROJECT_NAME} ${src})

target_link_libraries("${PROJECT_NAME}" PRIVATE ${SDL_LIBRARIES})

include(${CMAKE_CURRENT_SOURCE_DIR}/Tools/CMake/GroupSourcesByFolder.cmake)
GroupSourcesByFolder(${PROJECT_NAME})
