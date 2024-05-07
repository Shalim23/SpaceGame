//GENERATED FILE! DO NOT MODIFY DIRECTLY!
#pragma once
#include "../Types/TypesList.h"
#include "../Types/System.h"

#include "../Systems/GameplaySystem.h"
#include "../Systems/InputSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/WorldBoundsSystem.h"


using RegisteredSystems = TypesList
<
GameplaySystem,
InputSystem,
RenderSystem,
WorldBoundsSystem
>;

using Systems = SystemsList<RegisteredSystems>::type;