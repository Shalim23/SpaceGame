#pragma once
#include "World.h"
#include "../Generated/SystemsRegistry.h"

class SystemsManager
{

public:
    SystemsManager() = default;
    ~SystemsManager() = default;
    SystemsManager(const SystemsManager& other) = delete;
    SystemsManager(SystemsManager&& other) = delete;
    SystemsManager& operator=(const SystemsManager& other) = delete;
    SystemsManager& operator=(SystemsManager&& other) = delete;

    void init(World& world)
    {
        std::apply([&world, this](auto&&... system)
            {((system.init(world, *this)), ...);}, systems_);
    }

    void update(World& world, const double deltaTime)
    {
        std::apply([&world, deltaTime](auto&&... system)
            {((system.update(world, deltaTime)), ...); }, systems_);
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
    RegisteredSystems systems_;
};
