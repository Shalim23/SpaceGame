cmake_minimum_required(VERSION 3.28)

include(FetchContent)

FetchContent_Declare(SDL2 URL https://github.com/libsdl-org/SDL/releases/download/release-2.30.3/SDL2-2.30.3.tar.gz)
FetchContent_Declare(SDL2Image URL https://github.com/libsdl-org/SDL_image/releases/download/release-2.8.2/SDL2_image-2.8.2.tar.gz)
FetchContent_Declare(SDL2ttf URL https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.22.0/SDL2_ttf-2.22.0.tar.gz)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)

FetchContent_MakeAvailable(json)
FetchContent_MakeAvailable(SDL2)
FetchContent_MakeAvailable(SDL2Image)
FetchContent_MakeAvailable(SDL2ttf)
