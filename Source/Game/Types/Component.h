#pragma once
#include "Entity.h"
#include <tuple>
#include <vector>

template<typename T>
class Component
{
public:
    Component(const Entity entity, const size_t removeCallbackId)
        : owner_{ entity }
        , removeCallbackId_{ removeCallbackId }
        , component_{}
    {}

    Entity getOwningEntity() const { return owner_; }
    T& operator*() { return component_; }
    size_t getRemoveCallbackId() const { return removeCallbackId_; }

private:
    Entity owner_;
    size_t removeCallbackId_;
    T component_;
};

template<typename> struct ComponentsList;
template<template<typename...Args> typename t, typename ...Ts>
struct ComponentsList<t<Ts...>>
{
    using type = std::tuple<std::vector<Component<Ts>>...>;
};
