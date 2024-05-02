#include "RenderSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include "../Types/Exceptions.h"
#include "SDL.h"
#include "SDL_image.h"

void RenderSystem::init(World& w, SystemsManager& sm)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        showMessageBox("RenderSystem::init failed!", "Failed to init SDL!");
        throw SystemInitException{"RenderSystem::init failed!, Failed to init SDL!"};
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        showMessageBox("RenderSystem::init failed!", "Failed to init SDL_Image!");
        throw SystemInitException{ "RenderSystem::init failed!, Failed to init SDL_Image!" };
    }

    m_window = SDL_CreateWindow("Space Game", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 800, 600, 0/*SDL_WINDOW_FULLSCREEN_DESKTOP*/);
    if (!m_window)
    {
        showMessageBox("RenderSystem::init failed!", "Failed to create window!");
        throw SystemInitException{ "RenderSystem::init failed!, Failed to create window!" };
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (!m_renderer)
    {
        showMessageBox("RenderSystem::init failed!", "Failed to create renderer!");
        throw SystemInitException{ "RenderSystem::init failed!, Failed to create renderer!" };
    }
}

void RenderSystem::shutdown()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    IMG_Quit();
    SDL_Quit();
}

void RenderSystem::showMessageBox(const char* title, const char* message)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message, m_window);
}

SDL_Texture* RenderSystem::createTexture(const std::vector<char>& data) const
{
    return IMG_LoadTexture_RW(m_renderer, SDL_RWFromConstMem(data.data(), data.size()), 1);
}

SDL_Point RenderSystem::getTextureSize(SDL_Texture* texture) const
{
    SDL_Point size;
    SDL_QueryTexture(texture, nullptr, nullptr, &size.x, &size.y);
    return size;
}
