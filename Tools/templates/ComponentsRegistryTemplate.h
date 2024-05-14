//GENERATED FILE! DO NOT MODIFY DIRECTLY!
#pragma once
#include "../Types/TypesList.h"
#include "../Types/Component.h"
#include "AllComponents.h"
#include "ComponentType.h"


using RegisteredComponentsTypes = TypesList
<
{%- for t, c in components.items() %}
{{"Components<"}}{{t}}{{" ,"}}{{ c }}{{">"}}{{ "," if not loop.last }}
{%- endfor %}
>;

using RegisteredComponents = ComponentsList<RegisteredComponentsTypes>::type;
