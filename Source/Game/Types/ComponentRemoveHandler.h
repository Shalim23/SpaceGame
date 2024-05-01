#pragma once
#include <functional>
#include <cassert>
#include "Entity.h"

struct ComponentRemoveCallback
{
    explicit ComponentRemoveCallback(std::function<void()> callback, const size_t callback_id)
        : cb{callback}, id{callback_id}
    {
        assert(cb != nullptr);
    }

    size_t id;
    std::function<void()> cb;
};

struct ComponentRemoveHandler
{
    explicit ComponentRemoveHandler(const Entity _e)
        : e{ _e }
    {}
    Entity e;
    std::vector<ComponentRemoveCallback> destructors;
};
