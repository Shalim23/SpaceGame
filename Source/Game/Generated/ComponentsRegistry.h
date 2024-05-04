//GENERATED FILE! DO NOT MODIFY DIRECTLY!
#pragma once
#include "../Types/TypesList.h"
#include "../Types/Component.h"
#include "../Components/CameraComponent.h"
#include "../Components/InputComponent.h"
#include "../Components/RenderComponent.h"
#include "../Components/TransformComponent.h"


using RegisteredComponents = TypesList
<
CameraComponent,
InputComponent,
RenderComponent,
TransformComponent
>;

using Components = ComponentsList<RegisteredComponents>::type;
