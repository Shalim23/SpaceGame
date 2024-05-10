#pragma once
#include <numeric>

namespace fl
{
    bool inRange(const float value, const float minValue, const float maxValue)
    {
        constexpr float epsilon{ std::numeric_limits<float>::epsilon() };
        return (value >= minValue - epsilon && value <= maxValue + epsilon);
    }

    float lerp(const float startValue, const float endValue, const float delta)
    {
        return startValue + delta * (endValue - startValue);
    }
}