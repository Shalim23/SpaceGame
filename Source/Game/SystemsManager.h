#pragma once
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


    void init()
    {
        std::apply([](auto&&... system) {((system.init()), ...);}, m_systems);
    }

    template<typename T>
    T& getSystem()
    {
        return std::get<T>(m_systems);
    }

private:
    Systems m_systems;
};
