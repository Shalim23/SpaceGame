//GENERATED FILE! DO NOT MODIFY DIRECTLY!
#pragma once
#include "../Types/TypesList.h"
#include "../Types/System.h"
#include "AllGameSystems.h"


using RegisteredGameSystems = TypesList
<
GameplaySystem,
MenuSystem,
MovementSystem,
RandomSystem,
TextSystem,
WorldBoundsSystem
>;

using GameSystems = SystemsList<RegisteredGameSystems>::type;