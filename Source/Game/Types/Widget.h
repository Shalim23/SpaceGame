#pragma once
#include "RenderData.h"
#include <vector>
#include <functional>
#include <optional>


class Widget
{
    class Animation
    {
    public:
        explicit Animation(const Uint64 animationTimeMs,
            std::function<void(const float)> animationFunction);
        void run();
        bool isAnimationFinished() const;

    private:
        Uint64 animationTimeMs_{};
        Uint64 currentAnimationTimeMs_{};
        std::function<void(const float)> animationFunction_{};
    };

public:
    RenderData& updateRenderData();
    void addAnimation(const Uint64 animationTimeMs,
        std::function<void(const float)> animationFunction);
    
    void gatherRenderData(std::vector<const RenderData*>& data);
    bool hasAnimation() const;
    bool isAnimationFinished() const;

private:
    RenderData renderData_{};
    std::optional<Animation> animation_;
};
