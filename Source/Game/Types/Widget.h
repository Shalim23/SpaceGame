#pragma once
#include "RenderData.h"
#include <vector>
#include <functional>
#include <optional>

class WidgetAnimation
{
public:
    explicit WidgetAnimation(const Uint64 animationTimeMs,
        std::function<void(const float)> animationFunction)
        : animationTimeMs_{ animationTimeMs }
        , animationFunction_{animationFunction}
    {
        animationStartTimeMs_ = SDL_GetTicks64();
    }

    void run()
    {
        const Uint64 currentTimeMs{ SDL_GetTicks64() };
        const Uint64 timeSinceAnimationStart{ currentTimeMs - animationStartTimeMs_ };
        if (timeSinceAnimationStart < animationTimeMs_)
        {
            const float delta{ static_cast<float>(timeSinceAnimationStart) / animationTimeMs_ };
            animationFunction_(delta);
        }
    }

private:
    Uint64 animationTimeMs_{};
    Uint64 animationStartTimeMs_{};
    std::function<void(const float)> animationFunction_{};
};

class Widget
{
public:
    RenderData& updateRenderData() { return renderData_; }

    void addAnimation(const Uint64 animationTimeMs,
        std::function<void(const float)> animationFunction)
    {
        animation_.emplace(WidgetAnimation{animationTimeMs, animationFunction});
    }
    
    void gatherRenderData(std::vector<const RenderData*>& data)
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

private:
    RenderData renderData_{};
    std::vector<Widget> children_;
    std::optional<WidgetAnimation> animation_;
};
