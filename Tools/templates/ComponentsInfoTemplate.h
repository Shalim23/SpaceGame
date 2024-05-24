//GENERATED FILE! DO NOT MODIFY DIRECTLY!
#pragma once

template<typename T>
struct ComponentInfo
{
    const ComponentType componentType;
};

static constexpr auto ComponentInfos
{
    std::make_tuple(
{%- for t, c in components.items() %}
{{"ComponentInfo<"}}{{ c }}{{">{.componentType = "}}{{t}}{{"}"}}{{ "," if not loop.last }}
{%- endfor %}
    )
};
