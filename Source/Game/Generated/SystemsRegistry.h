//GENERATED FILE! DO NOT MODIFY DIRECTLY!
#pragma once
#include "../Types/TypesList.h"
#include "../Types/Component.h"
#include "AllSystems.h"


using RegisteredSystemsTypes = TypesList
<
DatabaseSystem,
GameplaySystem,
InputSystem,
MenuSystem,
MovementSystem,
RandomSystem,
RenderSystem,
WorldBoundsSystem
>;

using RegisteredSystems = ComponentsList<RegisteredSystemsTypes>::type;