#pragma once
#include "World.h"
#include "SystemsManager.h"
#include "Types/Input.h"

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
    SystemsManager systemsManager_;
    Input inputState_;

    InputSystem& inputSystem_;
    RenderSystem& renderSystem_;
    double deltaTime_{};
    uint8_t is_running_ : 1;
};