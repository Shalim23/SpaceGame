cmake_minimum_required(VERSION 3.28)

include(FetchContent)

FetchContent_Declare(SDL2 URL https://github.com/libsdl-org/SDL/releases/download/release-2.30.3/SDL2-2.30.3.tar.gz)
FetchContent_Declare(SDL2Image URL https://github.com/libsdl-org/SDL_image/releases/download/release-2.8.2/SDL2_image-2.8.2.tar.gz)
FetchContent_Declare(SDL2ttf URL https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.22.0/SDL2_ttf-2.22.0.tar.gz)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY  https://github.com/ocornut/imgui
    GIT_TAG         6ccc561a2ab497ad4ae6ee1dbd3b992ffada35cb
    GIT_PROGRESS    TRUE
)

FetchContent_GetProperties(imgui)
if (NOT imgui_POPULATED)
  FetchContent_Populate(imgui)
endif ()

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

add_library(imgui ${IMGUI_SOURCES} ${IMGUI_HEADERS})

FetchContent_MakeAvailable(json)
FetchContent_MakeAvailable(SDL2)
FetchContent_MakeAvailable(SDL2Image)
FetchContent_MakeAvailable(SDL2ttf)
FetchContent_MakeAvailable(imgui)
