//GENERATED FILE! DO NOT MODIFY DIRECTLY!
#pragma once
#include "../Types/TypesList.h"
#include "../Types/Component.h"

#include "../Components/MovementComponent.h"
#include "../Components/OutOfWorldBoundsComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/RenderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/UIRenderComponent.h"


using RegisteredComponents = TypesList
<
MovementComponent,
OutOfWorldBoundsComponent,
PlayerComponent,
RenderComponent,
TransformComponent,
UIRenderComponent
>;

using Components = ComponentsList<RegisteredComponents>::type;