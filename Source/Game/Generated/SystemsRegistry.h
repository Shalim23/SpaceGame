//GENERATED FILE! DO NOT MODIFY DIRECTLY!
#pragma once
#include "../Types/TypesList.h"
#include "../Types/System.h"
#include "../Systems/RenderSystem.h"


using RegisteredSystems = TypesList
<
RenderSystem
>;

using Systems = SystemsList<RegisteredSystems>::type;
