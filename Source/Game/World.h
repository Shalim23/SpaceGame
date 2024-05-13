#pragma once
#include "Generated/ComponentsRegistry.h"
#include "Types/Entity.h"
#include "Types/ComponentRemoveCallback.h"
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
        const Entity entity{ ++entityId_ };
        entities_.emplace_back(entity);

        componentRemoveCallbacks_.emplace_back(
            EntityComponentRemoveCallbacks{entity});

        return entity;
    }

    void destroyEntity(const Entity entity)
    {
        auto& componentRemoveCallbacks{ getComponentRemoveCallbacks(entity) };
        for (auto& callback : componentRemoveCallbacks.callbacks)
        {
            callback();
        }

        std::erase(entities_, entity);
        std::erase_if(componentRemoveCallbacks_,
            [entity](const EntityComponentRemoveCallbacks& callbacks)
                {return callbacks.entity == entity;});
    }

    template<typename T>
    T& addComponent(const Entity entity)
    {
        auto& components{ getComponents<T>() };
        auto iter{std::ranges::find_if(components,
            [entity](const Component<T>& component)
                { return entity == component.getOwningEntity(); })};
        if (iter != components.end())
        {
            assert(!"Entity can have only one instance of any component!");
            return *(*iter);
        }

        auto& componentRemoveCallbacks{ getComponentRemoveCallbacks(entity) };
        componentRemoveCallbacks.callbacks.emplace_back(
            ComponentRemoveCallback{
                [this, entity] { removeComponentInternal<T>(entity); },
                ++componentRemoveCallbackId_ });

        auto& new_component{ 
            components.emplace_back(Component<T>{entity, componentRemoveCallbackId_})
        };

        return *new_component;
    }

    template<typename T>
    T* tryGetComponent(const Entity entity)
    {
        auto& components{ getComponents<T>() };
        auto iter{ std::ranges::find_if(components,
            [entity](const Component<T>& component)
                { return entity == component.getOwningEntity(); }) };
        return iter != components.end() ? &(*(*iter)) : nullptr;
    }

    template<typename T>
    void forEach(std::function<void(const Entity, T&)> callback)
    {
        for (auto& component : getComponents<T>())
        {
            callback(component.getOwningEntity(), *component);
        }
    }

    template<typename T>
    void removeComponent(const Entity entity)
    {
        const auto removeCallbackId{removeComponentInternal<T>(entity)};

        auto& componentRemoveCallbacks{ getComponentRemoveCallbacks(entity) };
        const auto pred{ [removeCallbackId](const ComponentRemoveCallback& callback)
                { return callback.getId() == removeCallbackId; }};
        const auto result{std::erase_if(componentRemoveCallbacks.callbacks, pred)};
        assert(result == 1 && "No destruction callback was erased!");
    }

private:
    template<typename T>
    size_t removeComponentInternal(const Entity entity)
    {
        auto& components{ getComponents<T>() };
        const auto iter{ std::ranges::find_if(components,
            [entity](const Component<T> component)
                { return entity == component.getOwningEntity(); }) };
        if (iter == components.end())
        {
            assert(!"Trying to remove non-existent component!");
            return 0;
        }

        const auto removeCallbackId{iter->getRemoveCallbackId()};
        const auto index{ iter - components.begin() };
        std::swap(components[index], components.back());
        components.pop_back();

        return removeCallbackId;
    }

    EntityComponentRemoveCallbacks& getComponentRemoveCallbacks(const Entity entity)
    {
        auto iter{ std::ranges::find_if(componentRemoveCallbacks_,
                    [entity](const EntityComponentRemoveCallbacks& callback)
                        {return callback.entity == entity; }) };
        assert(iter != componentRemoveCallbacks_.end() && "No component remove callbacks found for entity!");
        return *iter;
    }

    template<typename T>
    std::vector<Component<T>>& getComponents()
    {
        return std::get<std::vector<Component<T>>>(components_);
    }

private:
    ComponentRemoveCallbacks componentRemoveCallbacks_;
    Components components_;
    std::vector<Entity> entities_;
    size_t entityId_{0};
    size_t componentRemoveCallbackId_{0};
};
