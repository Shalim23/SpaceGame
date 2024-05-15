#include "Widget.h"
#include "../Constants.h"

Widget::Animation::Animation(const Uint64 animationTimeMs,
    std::function<void(const float)> animationFunction)
    : animationTimeMs_{ animationTimeMs }
    , animationFunction_{ animationFunction }
{
}

void Widget::Animation::run()
{
    if (currentAnimationTimeMs_ < animationTimeMs_)
    {
        const float delta{ static_cast<float>(currentAnimationTimeMs_) / animationTimeMs_ };
        animationFunction_(delta);
    }

    currentAnimationTimeMs_ += constants::frameTimeMs;
}

bool Widget::Animation::isAnimationFinished() const
{
    return currentAnimationTimeMs_ > animationTimeMs_;
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

bool Widget::hasAnimation() const
{
    return animation_.has_value();
}

bool Widget::isAnimationFinished() const
{
    return animation_->isAnimationFinished();
}
