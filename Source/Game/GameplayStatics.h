#pragma once
#include "World.h"

namespace gameplayStatics
{
    static Component<PlayerComponent>& getPlayerComponent(World& world)
    {
        return world.getComponents<PlayerComponent>().at(0);
    }

    static GameStateType getCurrentGameState(World& world)
    {
        return world.getComponents<GameStateComponent>().at(0).instance.gameState;
    }
}
