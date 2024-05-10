#pragma once
#include "SDL.h"

namespace constants
{
    static constexpr Uint64 secondMs{ 1000 };
    static constexpr Uint64 frames{ 60 };
    static constexpr Uint64 frameTimeMs{ secondMs / frames };
    static constexpr float frameTimeMsF{ static_cast<float>(frameTimeMs) };
    static constexpr double frameTimeMsD{ static_cast<double>(frameTimeMs) };
    static constexpr double fullCircleDegreesD{ 360.0 };
}
