//GENERATED FILE! DO NOT MODIFY DIRECTLY!
#pragma once
#include "../Types/TypesList.h"
#include "../Types/Component.h"
#include "AllSystems.h"


using RegisteredSystemsTypes = TypesList
<
{%- for s in systems %}
{{ s }}{{ "," if not loop.last }}
{%- endfor %}
>;

using RegisteredSystems = ComponentsList<RegisteredSystemsTypes>::type;
