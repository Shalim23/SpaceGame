#pragma once
#include <functional>
#include <cassert>
#include "Entity.h"

struct ComponentRemoveCallback
{
    explicit ComponentRemoveCallback(std::function<void()> callback_, const size_t callback_id)
        : callback{callback_}, id{callback_id}
    {
        assert(callback != nullptr);
    }

    void operator()()
    {
        callback();
    }

    size_t id;
    std::function<void()> callback;
};

struct EntityComponentRemoveCallbacks
{
    explicit EntityComponentRemoveCallbacks(const Entity entity_)
        : entity{ entity_ }
    {}
    Entity entity;
    std::vector<ComponentRemoveCallback> callbacks;
};

using ComponentRemoveCallbacks = std::vector<EntityComponentRemoveCallbacks>;
