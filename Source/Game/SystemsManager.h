#pragma once
#include "World.h"
#include "Types/System.h"
#include "Generated/SystemsRegistry.h"

class SystemsManager
{

public:
    SystemsManager() = default;
    ~SystemsManager() = default;
    SystemsManager(const SystemsManager& other) = delete;
    SystemsManager(SystemsManager&& other) = delete;
    SystemsManager& operator=(const SystemsManager& other) = delete;
    SystemsManager& operator=(SystemsManager&& other) = delete;


    void init(World& w)
    {
        std::apply([&w, this](auto&&... system)
            {((system.init(w, *this)), ...);}, m_systems);
    }

    void update(World& w)
    {
        std::apply([&w](auto&&... system)
            {((system.update(w)), ...); }, m_systems);
    }

    void shutdown()
    {
        std::apply([](auto&&... system)
            {((system.shutdown()), ...); }, m_systems);
    }

    template<typename T>
    T& getSystem()
    {
        return std::get<T>(m_systems);
    }

private:
    Systems m_systems;
};
