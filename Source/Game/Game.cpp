#include "Game.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Utils.h"

#ifndef NDEBUG
#include "imgui_impl_sdl2.h"
#endif

Game::Game()
    : inputSystem_{systemsManager_.getSystem<InputSystem>()}
    , renderSystem_{systemsManager_.getSystem<RenderSystem>()}
    , is_running_{true}
{
}

void Game::init()
{
    initSDL();
    renderSystem_.createWindow();
    systemsManager_.getSystem<DataSystem>().init(world_, systemsManager_);

    systemsManager_.init(world_);
}

void Game::run()
{
    while (is_running_)
    {
        auto frameTimeStart{SDL_GetPerformanceCounter()};

        inputState_.clear();

        SDL_Event currentEvent;
        while (SDL_PollEvent(&currentEvent))
        {
#ifndef NDEBUG
            ImGui_ImplSDL2_ProcessEvent(&currentEvent);
#endif
            switch (currentEvent.type)
            {
            case SDL_QUIT:
            {
                is_running_ = false;
                return;
            }
            }

            inputState_.processEvent(currentEvent);
        }

        inputSystem_.processInput(inputState_, world_, deltaTime_);

        systemsManager_.update(world_, deltaTime_);
        world_.destroyEntities();

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
