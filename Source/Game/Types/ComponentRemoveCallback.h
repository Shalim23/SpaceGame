#pragma once
#include <functional>
#include <cassert>
#include "Entity.h"

class ComponentRemoveCallback
{
public:
    explicit ComponentRemoveCallback(std::function<void()> callback, const size_t callback_id)
        : callback_{callback_}, id_{callback_id}
    {
        assert(callback != nullptr);
    }

    void operator()()
    {
        callback_();
    }

    size_t getId() const { return id_; }

private:
    size_t id_;
    std::function<void()> callback_;
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
