#include "Game.h"
#include "SDL.h"
#include "Constants.h"


Game::Game()
    : m_is_running{true}
{
}

void Game::run()
{
    m_systems_manager.preinit(m_world);
    m_systems_manager.init(m_world);

    while (m_is_running)
    {
        const Uint64 frame_time_begin{ SDL_GetTicks64() };

        SDL_Event current_event{};
        while (SDL_PollEvent(&current_event))
        {
            switch (current_event.type)
            {
            case SDL_QUIT:
            {
                m_is_running = false;
                m_systems_manager.shutdown();
                return;
            }
            }
        }

        m_systems_manager.update(m_world);

        const Uint64 frame_time{ SDL_GetTicks64() - frame_time_begin };
        if (frame_time < FrameTimeMs)
        {
            SDL_Delay(FrameTimeMs - frame_time);
        }
    }
}
