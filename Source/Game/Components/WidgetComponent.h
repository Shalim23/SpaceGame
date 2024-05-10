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
    RenderData renderData{};
    std::vector<WidgetElement> children;
};

struct WidgetComponent
{
    WidgetLayer layer{WidgetLayer::COUNT};
    std::vector<WidgetElement> elements;
};