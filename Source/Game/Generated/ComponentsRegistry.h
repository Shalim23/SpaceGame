//GENERATED FILE! DO NOT MODIFY DIRECTLY!
#pragma once
#include "../Types/TypesList.h"
#include "../Types/Component.h"

#include "../Components/MovementComponent.h"
#include "../Components/OutOfWorldBoundsComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/WidgetComponent.h"


using RegisteredComponents = TypesList
<
MovementComponent,
OutOfWorldBoundsComponent,
PlayerComponent,
SpriteComponent,
TransformComponent,
WidgetComponent
>;

using Components = ComponentsList<RegisteredComponents>::type;