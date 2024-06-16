//GENERATED FILE! DO NOT MODIFY DIRECTLY!
#pragma once

enum class {{name}} : uint32_t
{
{%- for e in entries %}
{{"\t"}}{{ e }}{{ "," if not loop.last }}
{%- endfor %}
};
