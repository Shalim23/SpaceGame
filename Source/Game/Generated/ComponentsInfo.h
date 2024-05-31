//GENERATED FILE! DO NOT MODIFY DIRECTLY!
#pragma once

template<typename T>
struct ComponentInfo
{
    const ComponentType componentType;
};

static constexpr auto ComponentInfos
{
    std::make_tuple(
ComponentInfo<GameObjectComponent>{.componentType = ComponentType::GameObject},
ComponentInfo<GameStateComponent>{.componentType = ComponentType::GameState},
ComponentInfo<MovementComponent>{.componentType = ComponentType::Movement},
ComponentInfo<OutOfWorldBoundsComponent>{.componentType = ComponentType::OutOfWorldBounds},
ComponentInfo<PlayerComponent>{.componentType = ComponentType::Player},
ComponentInfo<RenderComponent>{.componentType = ComponentType::Render},
ComponentInfo<TransformComponent>{.componentType = ComponentType::Transform},
ComponentInfo<UIComponent>{.componentType = ComponentType::UI}
    )
};