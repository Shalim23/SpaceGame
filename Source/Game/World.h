#pragma once
#include "Generated/ComponentsRegistry.h"
#include "Types/Entity.h"
#include "Types/ComponentRemoveHandler.h"
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

    size_t getEntitiesAmount() const
    {
        return m_entities.size();
    }

    Entity createEntity()
    {
        const Entity e{ ++m_next_entity };
        m_entities.emplace_back(e);
        m_component_remove_handlers.emplace_back(ComponentRemoveHandler{e});
        return e;
    }

    void destroyEntity(const Entity e)
    {
        auto& component_destructors{ getComponentDestructors(e) };
        for (auto& d : component_destructors.destructors)
        {
            d.cb();
        }

        std::erase(m_entities, e);
        std::erase_if(m_component_remove_handlers, [e](const ComponentRemoveHandler& d){return d.e == e;});
    }

    template<typename T>
    T& addComponent(const Entity e)
    {
        auto& components{ getComponents<T>() };
        auto iter{std::ranges::find_if(components,
            [e](const Component<T> component){ return e == component.getOwner(); })};
        if (iter != components.end())
        {
            assert(false && "Entity can have only one instance of any component!");
            return iter->get();
        }

        auto& component_destructors{ getComponentDestructors(e) };
        component_destructors.destructors.emplace_back(
            ComponentRemoveCallback{
                [this, e] { removeComponentInternal<T>(e); },
                ++m_next_component_destruction_callback_id });

        auto& new_component{ 
            components.emplace_back(Component<T>{e, m_next_component_destruction_callback_id})
        };

        return new_component.get();
    }

    template<typename T>
    T* tryGetComponent(const Entity e)
    {
        auto& components{ getComponents<T>() };
        auto iter{ std::ranges::find_if(components,
            [e](const Component<T> component) { return e == component.getOwner(); }) };
        return iter != components.end() ? &iter->get() : nullptr;
    }

    template<typename T>
    void forEach(std::function<void(const Entity, T&)> f)
    {
        for (auto& component : getComponents<T>())
        {
            f(component.getOwner(), component.get());
        }
    }

    template<typename T>
    void removeComponent(const Entity e)
    {
        const auto component_destruction_callback_id{removeComponentInternal<T>(e)};

        auto& component_destructors{ getComponentDestructors(e) };
        auto pred{ [component_destruction_callback_id](const ComponentRemoveCallback& cdc)
                { return cdc.id == component_destruction_callback_id; } };
        const auto result{std::erase_if(component_destructors.destructors, pred)};
        assert(result == 1 && "No destruction callback was erased!");
    }

private:
    template<typename T>
    size_t removeComponentInternal(const Entity e)
    {
        auto& components{ getComponents<T>() };
        auto iter{ std::ranges::find_if(components,
            [e](const Component<T> component) { return e == component.getOwner(); }) };
        if (iter == components.end())
        {
            assert(false && "Trying to remove non-existent component!");
            return 0;
        }

        const auto component_destruction_callback_id{iter->getRemoveCallbackId()};
        const auto index{ iter - components.begin() };
        std::swap(components[index], components.back());
        components.pop_back();

        return component_destruction_callback_id;
    }

    ComponentRemoveHandler& getComponentDestructors(const Entity e)
    {
        auto iter{ std::ranges::find_if(m_component_remove_handlers,
                    [e](const ComponentRemoveHandler& cd) {return cd.e == e; }) };
        assert(iter != m_component_remove_handlers.end() && "No component remove handler found for entity!");
        return *iter;
    }

    template<typename T>
    auto& getComponents()
    {
        return std::get<std::vector<Component<T>>>(m_components);
    }

private:
    std::vector<ComponentRemoveHandler> m_component_remove_handlers;
    Components m_components;
    std::vector<Entity> m_entities;
    size_t m_next_entity{0};
    size_t m_next_component_destruction_callback_id{0};
};
