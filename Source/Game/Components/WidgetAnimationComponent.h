#pragma once

struct WidgetAnimationComponent
{
    float totalAnimationTime{};
    std::function<void(const float)> animation{nullptr};
    float currentAnimationTime{};
};
