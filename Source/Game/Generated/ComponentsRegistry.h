//GENERATED FILE! DO NOT MODIFY DIRECTLY!
#pragma once
#include "../Types/TypesList.h"
#include "../Types/Component.h"

#include "../Components/MovementComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/RenderComponent.h"
#include "../Components/TransformComponent.h"


using RegisteredComponents = TypesList
<
MovementComponent,
PlayerComponent,
RenderComponent,
TransformComponent
>;

using Components = ComponentsList<RegisteredComponents>::type;