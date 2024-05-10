#pragma once
#include "../Generated/TextureType.h"
#include "SDL.h"

struct Texture
{
    TextureType type;
    SDL_Texture* texture{nullptr};
    SDL_Point size;
};