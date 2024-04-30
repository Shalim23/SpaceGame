cmake_minimum_required(VERSION 3.28)

include(FetchContent)

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY  https://github.com/libsdl-org/SDL
    GIT_TAG         15ead9a40d09a1eb9972215cceac2bf29c9b77f6
    GIT_PROGRESS    TRUE
)

FetchContent_Declare(
    SDL2Image
    GIT_REPOSITORY  https://github.com/libsdl-org/SDL_image
    GIT_TAG         859357d2e5e82e002f54f440cc031e174e58e749
    GIT_PROGRESS    TRUE
)

FetchContent_MakeAvailable(SDL2)
FetchContent_MakeAvailable(SDL2Image)
