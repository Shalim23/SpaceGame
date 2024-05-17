#pragma once
#include "World.h"
#include "GameSystemsManager.h"

class Game
{
public:
    Game();
    bool init();
    void run();
    void shutdown();

private:
    bool initSDL();
    void shutdownSDL();

private:
    World world_;
    GameSystemsManager systems_manager_;
    uint8_t is_running_ : 1;
};