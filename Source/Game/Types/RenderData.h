#pragma once
#include "SDL.h"

struct RenderData
{
    SDL_Texture* texture{ nullptr };
    SDL_Point textureSize{};
    SDL_Rect sourceRect{};
    SDL_FRect destinationRect{};
    double rotation{};
    float scale{};
};
