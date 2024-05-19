#pragma once
#include "../Generated/TextureType.h"
#include "SDL.h"

struct TextureInfo
{
    SDL_Texture* texture{nullptr};
    SDL_Point size;
};
