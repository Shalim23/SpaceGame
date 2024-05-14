//GENERATED FILE! DO NOT MODIFY DIRECTLY!
#pragma once
#include "../Types/TypesList.h"
#include "../Types/Component.h"
#include "AllComponents.h"
#include "ComponentType.h"


using RegisteredComponentsTypes = TypesList
<
Components<ComponentType::GameState ,GameStateComponent>,
Components<ComponentType::Movement ,MovementComponent>,
Components<ComponentType::OutOfWorldBounds ,OutOfWorldBoundsComponent>,
Components<ComponentType::Player ,PlayerComponent>,
Components<ComponentType::Sprite ,SpriteComponent>,
Components<ComponentType::Transform ,TransformComponent>,
Components<ComponentType::Widget ,WidgetComponent>
>;

using RegisteredComponents = ComponentsList<RegisteredComponentsTypes>::type;