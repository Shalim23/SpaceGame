//GENERATED FILE! DO NOT MODIFY DIRECTLY!
#pragma once
#include "../ECS/Types/TypesList.h"
#include "../ECS/Types/Component.h"
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
UIComponent,
WidgetAnimationComponent
>;

using RegisteredComponents = ComponentsList<RegisteredComponentsTypes>::type;