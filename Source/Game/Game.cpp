#include "Game.h"
#include "SDL.h"


namespace
{
    constexpr Uint64 FRAME_MS{ 1000 / 60 };
}


Game::Game()
    : m_is_running{true}
{
}

void Game::run()
{
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

        const Uint64 frame_time_end{ SDL_GetTicks64() };
        const Uint64 frame_time{ frame_time_end - frame_time_begin };
        if (frame_time < FRAME_MS)
        {
            SDL_Delay(FRAME_MS - frame_time);
        }
    }
}
