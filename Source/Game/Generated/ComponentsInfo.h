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
ComponentInfo<GameStateComponent>{.componentType = ComponentType::GameState},
ComponentInfo<MovementComponent>{.componentType = ComponentType::Movement},
ComponentInfo<OutOfWorldBoundsComponent>{.componentType = ComponentType::OutOfWorldBounds},
ComponentInfo<PlayerComponent>{.componentType = ComponentType::Player},
ComponentInfo<SpriteComponent>{.componentType = ComponentType::Sprite},
ComponentInfo<TransformComponent>{.componentType = ComponentType::Transform},
ComponentInfo<WidgetComponent>{.componentType = ComponentType::Widget}
    )
};