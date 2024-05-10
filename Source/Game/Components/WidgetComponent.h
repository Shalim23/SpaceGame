#pragma once
#include "../Types/RenderData.h"
#include <vector>
#include <functional>
#include <optional>

enum class WidgetLayer
{
    HUD,
    EFFECTS,
    MENU,
    COUNT
};

class WidgetElementAnimation
{
public:
    explicit WidgetElementAnimation(const Uint64 animationTimeMs,
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

struct WidgetElement
{
    RenderData renderData{};
    std::vector<WidgetElement> children;
    std::optional<WidgetElementAnimation> animation;
};

class WidgetComponent
{
public:
    void setLayer(const WidgetLayer layer) { layer_ = layer; }
    WidgetLayer getLayer() const { return layer_; }
    WidgetElement& addElement() { return elements_.emplace_back(WidgetElement{}); }
    std::vector<const RenderData*> gatherRenderData()
    {
        std::vector<const RenderData*> data;
        for (auto& element : elements_)
        {
            gatherRenderDataRecursive(data, element);
        }
        return data;
    }

private:
    void gatherRenderDataRecursive(std::vector<const RenderData*>& data, WidgetElement& element) const
    {
        if (element.animation.has_value())
        {
            element.animation->run();
        }

        data.push_back(&element.renderData);
        for (auto& element : element.children)
        {
            gatherRenderDataRecursive(data, element);
        }
    }

private:
    WidgetLayer layer_{WidgetLayer::COUNT};
    std::vector<WidgetElement> elements_;
};