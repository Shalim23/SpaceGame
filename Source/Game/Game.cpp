#include "Game.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Constants.h"
#include "Utils.h"


Game::Game()
    : inputSystem_{systemsManager_.getSystem<InputSystem>()}
    , renderSystem_{systemsManager_.getSystem<RenderSystem>()}
    , is_running_{true}
{
}

void Game::init()
{
    initSDL();
    systemsManager_.getSystem<ResourcesSystem>().loadResources();

    systemsManager_.init(world_);
}

void Game::run()
{
    while (is_running_)
    {
        auto frameTimeStart{SDL_GetPerformanceCounter()};

        SDL_Event currentEvent;
        while (SDL_PollEvent(&currentEvent))
        {
            //#TODO mouse buttons
            switch (currentEvent.type)
            {
            case SDL_QUIT:
            {
                is_running_ = ~is_running_;
                return;
            }
            }
        }

        //set keyboard input state
        inputSystem_.processInput(inputState_);

        systemsManager_.update(world_, deltaTime_);

        renderSystem_.render();

        const auto frameTimeEnd{ SDL_GetPerformanceCounter() };
        deltaTime_ = (frameTimeEnd - frameTimeStart)
            / static_cast<double>(SDL_GetPerformanceFrequency());
    }
}

void Game::shutdown()
{
    systemsManager_.shutdown();
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
