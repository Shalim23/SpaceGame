#pragma once
#include "Generated/ComponentsRegistry.h"
#include "Types/Entity.h"
#include "FunctionsLibrary.h"
#include <cassert>
#include <vector>

class World
{

public:
    World() = default;
    ~World() = default;
    World(const World& other) = delete;
    World(World&& other) = delete;
    World& operator=(const World& other) = delete;
    World& operator=(World&& other) = delete;

    Entity createEntity()
    {
        return entities_.emplace_back(EntityInfo{ .entity = ++entityId_ }).entity;
    }

    void destroyEntity(const Entity entity)
    {
        const auto& entityInfo{ findEntity(entity) };
        for (const auto& callback : entityInfo.componentRemoveCallbacks)
        {
            callback(entity);
        }

        std::erase_if(entities_,
            [entity](const EntityInfo& info){ return info.entity == entity; });
    }

    template<ComponentType type>
    auto& addComponent(const Entity entity)
    {
        auto& entityInfo{findEntity(entity)};
        if (functionsLibrary::contains(entityInfo.components, type))
        {
            assert(!"Entity can have only one instance of any component!");
            return *tryGetComponent<type>(entity);
        }
        
        auto& new_component{ getComponents<type>().emplace_back()};
        new_component.entity = entity;
        entityInfo.components.emplace_back(type);
        entityInfo.componentRemoveCallbacks.emplace_back(
            [this](const Entity entity){ removeComponent<type>(entity); });

        return new_component.instance;
    }

    template<ComponentType type>
    auto* tryGetComponent(const Entity entity)
    {
        auto& components{ getComponents<type>() };
        auto iter{ std::ranges::find_if(components, [entity](const auto& component)
                { return entity == component.entity; }) };
        return iter != components.end() ? &iter->instance : nullptr;
    }

    template<ComponentType type>
    void removeComponent(const Entity entity)
    {
        auto& components{ getComponents<type>() };
        const auto iter{ std::ranges::find_if(components, [entity](const auto& component)
                { return entity == component.entity; }) };
        if (iter == components.end())
        {
            assert(!"Trying to remove non-existent component!");
            return;
        }

        const auto index{ iter - components.begin() };
        std::swap(components[index], components.back());
        components.pop_back();
    }

    template <ComponentType type>
    auto& getComponents()
    {
        return std::get<static_cast<size_t>(type)>(components_).instances;
    }

private:
    EntityInfo& findEntity(const Entity entity)
    {
        auto iter{ std::ranges::find_if(entities_, [entity](const EntityInfo& entityInfo)
        {
            return entityInfo.entity == entity;
        })};
        assert(iter != entities_.end() && "Unknown entity!");
        return *iter;
    }

private:
    RegisteredComponents components_;
    std::vector<EntityInfo> entities_;
    size_t entityId_{0};
};
