#include "Game.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Constants.h"


Game::Game()
    : is_running_{true}
{
}

void Game::init()
{
    initSDL();
    systems_manager_.preInit(world_);
    systems_manager_.init(world_);
    systems_manager_.postInit(world_);
}

void Game::run()
{
    while (is_running_)
    {
        const Uint64 frameTimeBegin{ SDL_GetTicks64() };

        SDL_Event currentEvent;
        while (SDL_PollEvent(&currentEvent))
        {
            switch (currentEvent.type)
            {
            case SDL_QUIT:
            {
                is_running_ = ~is_running_;
                return;
            }
            }
        }

        systems_manager_.update(world_);

        const Uint64 frameTime{ SDL_GetTicks64() - frameTimeBegin };
        if (frameTime < constants::frameTimeMs)
        {
            SDL_Delay(constants::frameTimeMs - frameTime);
        }
    }
}

void Game::shutdown()
{
    systems_manager_.shutdown();
    shutdownSDL();
}

void Game::initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        utils::showMessageBox(__FUNCTION__, "Failed to init SDL!");
        throw std::exception{};
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        utils::showMessageBox(__FUNCTION__, "Failed to init SDL_Image!");
        throw std::exception{};
    }

    if (TTF_Init() != 0)
    {
        utils::showMessageBox(__FUNCTION__, "Failed to init SDL TTF!");
        throw std::exception{};
    }
}

void Game::shutdownSDL()
{
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
