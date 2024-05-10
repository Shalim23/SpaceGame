cmake_minimum_required(VERSION 3.28)

include(FetchContent)

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY  https://github.com/libsdl-org/SDL
    GIT_TAG         fb1497566c5a05e2babdcf45ef0ab5c7cca2c4ae
    GIT_PROGRESS    TRUE
)

FetchContent_Declare(
    SDL2Image
    GIT_REPOSITORY  https://github.com/libsdl-org/SDL_image
    GIT_TAG         abcf63aa71b4e3ac32120fa9870a6500ddcdcc89
    GIT_PROGRESS    TRUE
)

FetchContent_MakeAvailable(SDL2)
FetchContent_MakeAvailable(SDL2Image)
