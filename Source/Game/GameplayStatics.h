#pragma once
#include "World.h"

namespace gameplayStatics
{
    static Component<PlayerComponent>& getPlayerComponent(World& world)
    {
        return world.getComponents<ComponentType::Player>().at(0);
    }

    static GameStateType getCurrentGameState(World& world)
    {
        return world.getComponents<ComponentType::GameState>().at(0).instance.gameState;
    }
}
