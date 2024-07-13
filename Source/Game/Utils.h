#pragma once
#include "SDL.h"
#include <numeric>
#include <chrono>

namespace utils
{
    static std::string_view getDataFilePath()
    {
        return "Data/data.bin";
    }
    
    static void showMessageBox(const std::string_view title, const std::string_view message)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title.data(), message.data(), nullptr);
    }

    static bool inRange(const float value, const SDL_FPoint minMaxValue)
    {
        constexpr float epsilon{ std::numeric_limits<float>::epsilon() };
        return (value >= minMaxValue.x - epsilon && value <= minMaxValue.y + epsilon);
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
    
    template<typename C, typename T>
    static bool contains(const C& container, const T& value)
    {
        const auto iter{std::ranges::find(container, value)};
        return iter != container.end();
    }

    template<typename C, typename P>
    static bool containsIf(const C& container, const P& predicate)
    {
        const auto iter{ std::ranges::find_if(container, predicate) };
        return iter != container.end();
    }

    static unsigned int getCurrentTimeMs()
    {
        const auto now{std::chrono::system_clock::now()};
        const auto duration{std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch())};
        return static_cast<unsigned int>(duration.count());
    }

    constexpr uint32_t hashString(std::string_view str)
    {
        auto hashFunc{[](std::string_view str)
        {
            uint32_t hash = 0x811c9dc5; // FNV offset basis
            for (char ch : str)
            {
                hash ^= static_cast<uint32_t>(ch);
                hash *= 0x01000193; // FNV prime
            }
            return hash;
        }};
        return hashFunc(str);
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

static SDL_FPoint operator*(const SDL_FPoint& lhs, const float value)
{
    return SDL_FPoint{ .x = lhs.x * value, .y = lhs.y * value };
}

static SDL_FPoint operator/(const SDL_FPoint& lhs, const float value)
{
    return SDL_FPoint{ .x = lhs.x / value, .y = lhs.y / value };
}

static SDL_Point operator/(const SDL_Point& lhs, const int value)
{
    return SDL_Point{ .x = lhs.x / value, .y = lhs.y / value };
}
