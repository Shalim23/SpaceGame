#pragma once
#include "SDL.h"

struct RenderData
{
    SDL_Texture* texture{ nullptr };
    SDL_Point texture_size{};
    SDL_Rect src{};
    SDL_FRect dst{};
    double rotation{};
};
