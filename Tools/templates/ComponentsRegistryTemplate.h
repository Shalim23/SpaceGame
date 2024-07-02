//GENERATED FILE! DO NOT MODIFY DIRECTLY!
#pragma once
#include "../ECS/Types/TypesList.h"
#include "../ECS/Types/Component.h"
#include "AllComponents.h"


using RegisteredComponentsTypes = TypesList
<
{%- for t, c in components.items() %}
{{ c }}{{ "," if not loop.last }}
{%- endfor %}
>;

using RegisteredComponents = ComponentsList<RegisteredComponentsTypes>::type;
