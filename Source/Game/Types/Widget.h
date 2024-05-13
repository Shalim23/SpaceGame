#pragma once
#include "RenderData.h"
#include <vector>
#include <functional>
#include <optional>

class WidgetAnimation
{
public:
    explicit WidgetAnimation(const Uint64 animationTimeMs,
        std::function<void(const float)> animationFunction);
    void run();

private:
    Uint64 animationTimeMs_{};
    Uint64 animationStartTimeMs_{};
    std::function<void(const float)> animationFunction_{};
};

class Widget
{
public:
    RenderData& updateRenderData();
    void addAnimation(const Uint64 animationTimeMs,
        std::function<void(const float)> animationFunction);
    
    void gatherRenderData(std::vector<const RenderData*>& data);

private:
    RenderData renderData_{};
    std::vector<Widget> children_;
    std::optional<WidgetAnimation> animation_;
};
