#pragma once
#include "Entity.h"
#include <tuple>
#include <vector>

template<typename T>
class Component
{
public:
    Component(const Entity entity, const size_t remove_callback_id)
        : m_owner{ entity }
        , m_remove_callback_id{ remove_callback_id }
        , m_instance{}
    {}

    Entity getOwner() const { return m_owner; }
    T& get() { return m_instance; }
    size_t getRemoveCallbackId() const { return m_remove_callback_id; }

private:
    Entity m_owner;
    size_t m_remove_callback_id;
    T m_instance;
};

template<typename> struct ComponentsList;
template<template<typename...Args> typename t, typename ...Ts>
struct ComponentsList<t<Ts...>>
{
    using type = std::tuple<std::vector<Component<Ts>>...>;
};
