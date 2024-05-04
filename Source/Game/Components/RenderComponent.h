#pragma once
#include "SDL.h"

enum class RenderLayer
{
    BACKGROUND,
    PLAYER,
    COUNT,
};

struct RenderComponent
{
    RenderLayer layer;
    SDL_Texture* texture{nullptr};
    SDL_Point texture_size;
    SDL_Rect src;
    SDL_Rect dst;
    double rotation{};
};
