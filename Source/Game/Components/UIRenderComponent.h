#pragma once
#include "SDL.h"

enum class UIRenderLayer
{
    HUD,
    OUT_OF_WORLD_BOUNDS_EFFECT,
    MENU,
    COUNT,
};

struct UIRenderComponent
{
    UIRenderLayer layer;
    SDL_Texture* texture{ nullptr };
    SDL_Rect src;
    SDL_FRect dst;
    double rotation{};
};
