#pragma once
#include "World.h"

class Game
{
public:
    Game();
    void run();

private:
    World m_world{};
};