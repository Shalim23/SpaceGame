//GENERATED FILE! DO NOT MODIFY DIRECTLY!
#pragma once
#include "../Types/TypesList.h"
#include "../Types/Component.h"
#include "AllComponents.h"


using RegisteredComponents = TypesList
<
{%- for c in components %}
{{ c }}{{ "," if not loop.last }}
{%- endfor %}
>;

using Components = ComponentsList<RegisteredComponents>::type;
