#pragma once
#include "../Types/RenderData.h"

enum class SpriteLayer
{
    BACKGROUND,
    PLAYER,
    COUNT,
};

struct SpriteComponent
{
    SpriteLayer layer{SpriteLayer::COUNT};
    RenderData render_data{};
};
