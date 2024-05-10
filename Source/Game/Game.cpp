#include "Game.h"
#include "SDL.h"
#include "Constants.h"


Game::Game()
    : is_running_{true}
{
}

void Game::run()
{
    systems_manager_.init(world_);
    systems_manager_.postInit(world_);

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
                systems_manager_.shutdown();
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
