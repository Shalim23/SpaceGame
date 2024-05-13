#pragma once
#include <numeric>

namespace fl
{
    static bool inRange(const float value, const float minValue, const float maxValue)
    {
        constexpr float epsilon{ std::numeric_limits<float>::epsilon() };
        return (value >= minValue - epsilon && value <= maxValue + epsilon);
    }

    static float lerp(const float startValue, const float endValue, const float delta)
    {
        return startValue + delta * (endValue - startValue);
    }

    static SDL_Rect makeRect(const SDL_Point& xy, const SDL_Point& wh)
    {
        return SDL_Rect{.x = xy.x, .y = xy.y, .w = wh.x, .h = wh.y};
    }

    static SDL_FRect makeRect(const SDL_FPoint& xy, const SDL_FPoint& wh)
    {
        return SDL_FRect{ .x = xy.x, .y = xy.y, .w = wh.x, .h = wh.y };
    }
}

static SDL_FPoint operator*(const SDL_FPoint& lhs, const SDL_Point& rhs)
{
    return SDL_FPoint{ .x = lhs.x * rhs.x, .y = lhs.y * rhs.y };
}

static SDL_FPoint operator/(const SDL_FPoint& lhs, const SDL_FPoint& rhs)
{
    return SDL_FPoint{ .x = lhs.x / rhs.x, .y = lhs.y / rhs.y };
}