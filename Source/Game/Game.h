#pragma once
#include "World.h"
#include "SystemsManager.h"

class Game
{
public:
    Game();
    void init();
    void run();
    void shutdown();

private:
    void initSDL();
    void shutdownSDL();

private:
    World world_;
    SystemsManager systems_manager_;
    uint8_t is_running_ : 1;
};