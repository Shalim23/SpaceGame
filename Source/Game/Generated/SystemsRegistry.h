//GENERATED FILE! DO NOT MODIFY DIRECTLY!
#pragma once
#include "../Types/TypesList.h"
#include "../Types/System.h"
#include "../Systems/TestSystem.h"


using RegisteredSystems = TypesList
<
TestSystem
>;

using Systems = SystemsList<RegisteredSystems>::type;
