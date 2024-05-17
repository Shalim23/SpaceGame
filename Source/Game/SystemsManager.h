#pragma once
#include "World.h"
#include "SystemsRegistry.h"

class SystemsManager
{

public:
    SystemsManager() = default;
    ~SystemsManager() = default;
    SystemsManager(const SystemsManager& other) = delete;
    SystemsManager(SystemsManager&& other) = delete;
    SystemsManager& operator=(const SystemsManager& other) = delete;
    SystemsManager& operator=(SystemsManager&& other) = delete;

    void preInit(World& world)
    {
        std::apply([&world, this](auto&&... system)
            {((system.preInit(world, *this)), ...); }, systems_);
    }

    void init(World& world)
    {
        std::apply([&world, this](auto&&... system)
            {((system.init(world, *this)), ...);}, systems_);
    }

    void postInit(World& world)
    {
        std::apply([&world, this](auto&&... system)
            {((system.postInit(world, *this)), ...); }, systems_);
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
    Systems systems_;
};
