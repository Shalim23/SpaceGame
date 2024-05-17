//GENERATED FILE! DO NOT MODIFY DIRECTLY!
#pragma once
#include "../Types/TypesList.h"
#include "../Types/Component.h"
#include "AllSystems.h"


using RegisteredSystemsTypes = TypesList
<
GameplaySystem,
InputSystem,
MenuSystem,
MovementSystem,
RandomSystem,
RenderSystem,
ResourcesSystem,
TextSystem,
WorldBoundsSystem
>;

using RegisteredSystems = ComponentsList<RegisteredSystemsTypes>::type;