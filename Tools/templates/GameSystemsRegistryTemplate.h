//GENERATED FILE! DO NOT MODIFY DIRECTLY!
#pragma once
#include "../Types/TypesList.h"
#include "../Types/System.h"
#include "AllGameSystems.h"


using RegisteredGameSystems = TypesList
<
{%- for s in systems %}
{{ s }}{{ "," if not loop.last }}
{%- endfor %}
>;

using GameSystems = SystemsList<RegisteredGameSystems>::type;
