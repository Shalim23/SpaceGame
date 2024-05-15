#pragma once
#include "Types/TypesList.h"
#include "Types/System.h"
#include "Generated/AllSystems.h"

// order is important here
using RegisteredSystems = TypesList
<
InputSystem,

MovementSystem,
GameplaySystem,
WorldBoundsSystem,
TextSystem,
MenuSystem,
RandomSystem,

RenderSystem
>;

using Systems = SystemsList<RegisteredSystems>::type;
