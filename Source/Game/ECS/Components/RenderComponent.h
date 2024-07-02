#pragma once
#include "SDL.h"

enum class RenderLayer
{
    Background,
    Player,
    HUD,
    UIEffects,
    Count,
};

struct RenderData
{
    SDL_Texture* texture{ nullptr };
    SDL_Point textureSize{};
    SDL_Rect sourceRect{};
    SDL_FRect destinationRect{};
    double rotation{};
    float scale{ 1.0f };
};

struct RenderComponent
{
    RenderLayer layer{RenderLayer::Count};
    RenderData data{};
};
