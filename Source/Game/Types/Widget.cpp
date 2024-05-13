#include "Widget.h"

WidgetAnimation::WidgetAnimation(const Uint64 animationTimeMs,
    std::function<void(const float)> animationFunction)
    : animationTimeMs_{ animationTimeMs }
    , animationFunction_{ animationFunction }
{
    animationStartTimeMs_ = SDL_GetTicks64();
}

void WidgetAnimation::run()
{
    const Uint64 currentTimeMs{ SDL_GetTicks64() };
    const Uint64 timeSinceAnimationStart{ currentTimeMs - animationStartTimeMs_ };
    if (timeSinceAnimationStart < animationTimeMs_)
    {
        const float delta{ static_cast<float>(timeSinceAnimationStart) / animationTimeMs_ };
        animationFunction_(delta);
    }
}

RenderData& Widget::updateRenderData()
{
    return renderData_;
}

void Widget::addAnimation(const Uint64 animationTimeMs, std::function<void(const float)> animationFunction)
{
    animation_.emplace(WidgetAnimation{ animationTimeMs, animationFunction });
}

void Widget::gatherRenderData(std::vector<const RenderData*>& data)
{
    if (animation_.has_value())
    {
        animation_->run();
    }

    data.push_back(&renderData_);
    for (auto& widget : children_)
    {
        widget.gatherRenderData(data);
    }
}
