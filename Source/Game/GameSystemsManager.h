#pragma once
#include "World.h"
#include "Systems/GameSystemsRegistry.h"

class GameSystemsManager
{

public:
    GameSystemsManager() = default;
    ~GameSystemsManager() = default;
    GameSystemsManager(const GameSystemsManager& other) = delete;
    GameSystemsManager(GameSystemsManager&& other) = delete;
    GameSystemsManager& operator=(const GameSystemsManager& other) = delete;
    GameSystemsManager& operator=(GameSystemsManager&& other) = delete;

    void init(World& world)
    {
        std::apply([&world, this](auto&&... system)
            {((system.init(world, *this)), ...);}, systems_);
    }

    void update(World& world)
    {
        std::apply([&world](auto&&... system)
            {((system.update(world)), ...); }, systems_);
    }

    void shutdown()
    {
        std::apply([](auto&&... system)
            {((system.shutdown()), ...); }, systems_);
    }

    template<typename T>
    T& getSystem()
    {
        return std::get<T>(systems_);
    }

private:
    GameSystems systems_;
};
