//GENERATED FILE! DO NOT MODIFY DIRECTLY!
#pragma once
#include "../ECS/Types/TypesList.h"
#include "../ECS/Types/System.h"
#include "AllSystems.h"


using RegisteredSystemsTypes = TypesList
<
DataSystem,
GameplaySystem,
InputSystem,
MenuSystem,
MovementSystem,
RandomSystem,
RenderSystem,
ViewportSystem,
WorldBoundsSystem
>;

using RegisteredSystems = SystemsList<RegisteredSystemsTypes>::type;