cmake_minimum_required(VERSION 3.28)

include(FetchContent)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY  https://github.com/ocornut/imgui
    GIT_TAG         e391fe2e66eb1c96b1624ae8444dc64c23146ef4
    GIT_PROGRESS    TRUE
)

FetchContent_GetProperties(imgui)
if (NOT imgui_POPULATED)
  FetchContent_Populate(imgui)
endif ()

FetchContent_MakeAvailable(imgui)

file(GLOB IMGUI_SOURCES
  "${imgui_SOURCE_DIR}/*.cpp"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.cpp"
)

file(GLOB IMGUI_HEADERS
  "${imgui_SOURCE_DIR}/*.h"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.h"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.h"
)

add_library(imgui_lib STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS})
target_include_directories(imgui_lib PUBLIC ${imgui_SOURCE_DIR} "${imgui_SOURCE_DIR}/backends")
target_link_libraries(imgui_lib PUBLIC SDL2)
