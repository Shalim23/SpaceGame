#pragma once
#include "../Types/RenderData.h"
#include <vector>

enum class WidgetLayer
{
    HUD,
    EFFECTS,
    MENU,
    COUNT
};

struct WidgetElement
{
    RenderData render_data{};
};

struct WidgetComponent
{
    WidgetLayer layer{WidgetLayer::COUNT};
    std::vector<WidgetElement> elements;
};