//GENERATED FILE! DO NOT MODIFY DIRECTLY!
#pragma once
#include "../Types/TypesList.h"
#include "../Types/Component.h"
#include "AllComponents.h"


using RegisteredComponentsTypes = TypesList
<
GameObjectComponent,
GameStateComponent,
MovementComponent,
OutOfWorldBoundsComponent,
PlayerComponent,
RenderComponent,
TransformComponent,
UIComponent
>;

using RegisteredComponents = ComponentsList<RegisteredComponentsTypes>::type;