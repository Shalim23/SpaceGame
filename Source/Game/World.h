#pragma once
#include "Generated/ComponentsRegistry.h"
#include "Generated/ComponentsInfo.h"
#include "Types/Entity.h"
#include "Utils.h"
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
        const auto [iter, result]{entities_.insert({ ++entityId_, {} })};
        assert(result);
        const auto& [entity, components]{*iter};
        return entity;
    }

    void queueEntityToDestoy(const Entity entity)
    {
        pendingDestroyEntities_.push_back(entity);
    }

    template <typename... Comps>
    void addComponents(const Entity entity)
    {
        (addComponent<Comps>(entity), ...);
    }

    template<typename T>
    T& addComponent(const Entity entity)
    {
        assert(entities_.contains(entity));

        auto& [components, indexes] { getComponentsInternal<T>()};
        if (indexes.contains(entity))
        {
            assert(!"Entity can have only one instance of any component!");
            return components[indexes[entity]].instance;
        }
        
        auto& new_component{ components.emplace_back(Component<T>{.entity = entity})};
        indexes[entity] = components.size() - 1;

        entities_[entity].emplace_back([this](const Entity entity){ removeComponent<T>(entity); });

        return new_component.instance;
    }

    template <typename T>
    bool hasComponent(const Entity entity)
    {
        return getComponentsInternal<T>().entityToComponentIndex.contains(entity);
    }

    template<typename T>
    auto* tryGetComponent(const Entity entity)
    {
        auto& [components, indexes] { getComponentsInternal<T>() };
        return indexes.contains(entity) ?
            &components[indexes[entity]].instance : nullptr;
    }

    template<typename T>
    void removeComponent(const Entity entity)
    {
        auto& [components, indexes]{ getComponentsInternal<T>() };
        const auto iter{ indexes.find(entity) };
        if (iter == indexes.end())
        {
            assert(!"Trying to remove non-existent component!");
            return;
        }

        const auto& [foundEntity, componentIndex]{*iter};
        const auto compoinentsSize{ components.size() };
        assert(compoinentsSize - 1 >= componentIndex);
        if (compoinentsSize > 1)
        {
            std::swap(components[componentIndex], components.back());
            indexes[components[componentIndex].entity] = componentIndex;
        }

        components.pop_back();
        indexes.erase(iter);
    }

    template <typename T>
    std::vector<Component<T>>& getComponents()
    {
        return std::get<Components<T>>(components_).instances;
    }

    template <typename T>
    ComponentType getComponentType()
    {
        return std::get<ComponentInfo<T>>(ComponentInfos).componentType;
    }

    void destroyEntities()
    {
        if (pendingDestroyEntities_.size() == 0)
        {
            return;
        }
        
        for (const auto entity : pendingDestroyEntities_)
        {
            const auto iter{ entities_.find(entity) };
            assert(iter != entities_.end());
            const auto& [found_entity, componentRemoveCallbacks] {*iter};
            for (const auto& callback : componentRemoveCallbacks)
            {
                callback(entity);
            }

            entities_.erase(iter);
        }

        pendingDestroyEntities_.clear();
    }

private:
    template <typename T>
    auto& getComponentsInternal()
    {
        return std::get<Components<T>>(components_);
    }

private:
    RegisteredComponents components_;
    std::unordered_map<Entity, std::vector<std::function<void(const Entity)>>> entities_;
    std::vector<Entity> pendingDestroyEntities_;
    size_t entityId_{0};
};
