#pragma once
#include <tuple>

template<typename> struct SystemsList;
template<template<typename...Args> typename t, typename ...Ts>
struct SystemsList<t<Ts...>>
{
    using type = std::tuple<Ts...>;
};
