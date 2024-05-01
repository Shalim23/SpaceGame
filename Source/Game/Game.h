#pragma once
#include "World.h"
#include "SystemsManager.h"

class Game
{
public:
    Game();
    void run();

private:
    World m_world{};
    SystemsManager m_systems_manager{};
    uint8_t m_is_running : 1;
};