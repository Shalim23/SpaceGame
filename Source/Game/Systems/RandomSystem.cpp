#include "RandomSystem.h"
#include "../World.h"
#include "../SystemsManager.h"

void RandomSystem::init(World& world, SystemsManager& systemsManager)
{
    generator_.seed(functionsLibrary::getCurrentTimeMs());
}

int RandomSystem::getRandomValue(const SDL_Point& range)
{
    std::uniform_int_distribution<int> distribution{range.x, range.y - 1};
    return distribution(generator_);
}

float RandomSystem::getRandomValue(const SDL_FPoint& range)
{
    std::uniform_real_distribution<float> distribution(range.x, range.y);
    return distribution(generator_);
}

SDL_Color RandomSystem::getRandomColor()
{
    constexpr SDL_Point colorMinMax{.x = 0, .y = 255};
    return SDL_Color{
    .r = static_cast<Uint8>(getRandomValue(colorMinMax)),
    .g = static_cast<Uint8>(getRandomValue(colorMinMax)),
    .b = static_cast<Uint8>(getRandomValue(colorMinMax)),
    .a = 255
    };
}
