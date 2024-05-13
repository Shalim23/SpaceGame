#include "Widget.h"

Widget::Animation::Animation(const Uint64 animationTimeMs,
    std::function<void(const float)> animationFunction)
    : animationTimeMs_{ animationTimeMs }
    , animationFunction_{ animationFunction }
{
    animationStartTimeMs_ = SDL_GetTicks64();
}

void Widget::Animation::run()
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
    animation_.emplace(Animation{ animationTimeMs, animationFunction });
}

void Widget::gatherRenderData(std::vector<const RenderData*>& data)
{
    if (animation_.has_value())
    {
        animation_->run();
    }

    data.push_back(&renderData_);
}
