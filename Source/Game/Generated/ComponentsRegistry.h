//GENERATED FILE! DO NOT MODIFY DIRECTLY!
#pragma once
#include "../Types/TypesList.h"
#include "../Types/Component.h"
#include "AllComponents.h"
#include "ComponentType.h"


using RegisteredComponentsTypes = TypesList
<
GameStateComponent,
MovementComponent,
OutOfWorldBoundsComponent,
PlayerComponent,
SpriteComponent,
TransformComponent,
WidgetComponent
>;

using RegisteredComponents = ComponentsList<RegisteredComponentsTypes>::type;


//#TODO separate file
using ComponentInfosList = TypesList
<
ComponentInfo<GameStateComponent>,
ComponentInfo<MovementComponent>,
ComponentInfo<OutOfWorldBoundsComponent>,
ComponentInfo<PlayerComponent>,
ComponentInfo<SpriteComponent>,
ComponentInfo<TransformComponent>,
ComponentInfo<WidgetComponent>
>;

using ComponentInfos = ComponentInfoList<ComponentInfosList>::type;

static ComponentInfos getComponentInfos()
{
    return {
        ComponentInfo<GameStateComponent>{.componentType = ComponentType::GameState},
        ComponentInfo<MovementComponent>{.componentType = ComponentType::Movement},
        ComponentInfo<OutOfWorldBoundsComponent>{.componentType = ComponentType::OutOfWorldBounds},
        ComponentInfo<PlayerComponent>{.componentType = ComponentType::Player},
        ComponentInfo<SpriteComponent>{.componentType = ComponentType::Sprite},
        ComponentInfo<TransformComponent>{.componentType = ComponentType::Transform},
        ComponentInfo<WidgetComponent>{.componentType = ComponentType::Widget}
    };
}