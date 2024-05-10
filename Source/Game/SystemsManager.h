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

    void preInit(World& w)
    {
        std::apply([&w, this](auto&&... system)
            {((system.preInit(w, *this)), ...); }, systems_);
    }

    void init(World& w)
    {
        std::apply([&w, this](auto&&... system)
            {((system.init(w, *this)), ...);}, systems_);
    }

    void update(World& w)
    {
        std::apply([&w](auto&&... system)
            {((system.update(w)), ...); }, systems_);
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
