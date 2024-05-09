#pragma once
#include "SDL.h"

enum class SpriteLayer
{
    BACKGROUND,
    PLAYER,
    COUNT,
};

struct SpriteComponent
{
    SpriteLayer layer;
    SDL_Texture* texture{nullptr};
    SDL_Point texture_size;
    SDL_Rect src;
    SDL_FRect dst;
    double rotation{};
};
