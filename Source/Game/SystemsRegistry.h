#pragma once
#include "Types/TypesList.h"
#include "Types/System.h"

#include "Systems/GameplaySystem.h"
#include "Systems/InputSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/WorldBoundsSystem.h"

// order is important here
using RegisteredSystems = TypesList
<
InputSystem,

GameplaySystem,
WorldBoundsSystem,

RenderSystem
>;

using Systems = SystemsList<RegisteredSystems>::type;
