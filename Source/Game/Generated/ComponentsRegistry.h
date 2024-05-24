//GENERATED FILE! DO NOT MODIFY DIRECTLY!
#pragma once
#include "../Types/TypesList.h"
#include "../Types/Component.h"
#include "AllComponents.h"


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