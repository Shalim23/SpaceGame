#pragma once
#include "Entity.h"
#include "../Generated/ComponentType.h"
#include <tuple>
#include <vector>
#include <functional>

template<typename T>
struct Component
{
    Entity entity;
    T instance{};
};

template<typename T>
struct Components
{
    std::vector<Component<T>> instances;
    std::unordered_map<Entity, size_t> entityToComponentIndex;
};

template<typename> struct ComponentsList;
template<template<typename...Args> typename t, typename ...Ts>
struct ComponentsList<t<Ts...>>
{
    using type = std::tuple<Components<Ts>...>;
};

template<typename T>
struct ComponentInfo
{
    const ComponentType componentType;
};

template<typename> struct ComponentInfoList;
template<template<typename...Args> typename t, typename ...Ts>
struct ComponentInfoList<t<Ts...>>
{
    using type = std::tuple<Ts...>;
};
