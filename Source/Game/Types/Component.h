#pragma once
#include "Entity.h"
#include "../Generated/ComponentType.h"
#include <tuple>
#include <vector>
#include <functional>

struct EntityInfo
{
    Entity entity;
    std::vector<ComponentType> components;
    std::vector<std::function<void(const Entity)>> componentRemoveCallbacks;
};

template<typename T>
struct Component
{
    Entity entity{};
    T instance{};
};

template<ComponentType value, typename T>
struct Components
{
    const ComponentType componentType = value;
    std::vector<Component<T>> instances;
};

template<typename> struct ComponentsList;
template<template<typename...Args> typename t, typename ...Ts>
struct ComponentsList<t<Ts...>>
{
    using type = std::tuple<Ts...>;
};

