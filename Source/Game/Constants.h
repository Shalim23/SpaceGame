#pragma once
#include "SDL.h"

static constexpr Uint64 SecondMs{ 1000 };
static constexpr Uint64 Frames{ 60 };
static constexpr Uint64 FrameTimeMs{ SecondMs / Frames };
static constexpr float FrameTimeMsF{ static_cast<float>(FrameTimeMs) };
static constexpr double FrameTimeMsD{ static_cast<double>(FrameTimeMs) };
static constexpr double FullCircleDegreesD{ 360.0 };
