#include "InputSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include "../Constants.h"
#include <iostream>

void InputSystem::update(World& w)
{
    int len;
    const Uint8* keyboardState{SDL_GetKeyboardState(&len)};

    w.forEach<InputComponent>([this, &w, &keyboardState]
        (const Entity ent, InputComponent& comp)
        {
            auto& transform{ *w.tryGetComponent<TransformComponent>(ent) };
            processRotation(keyboardState, transform);
        });
}

void InputSystem::processRotation(const Uint8* keyboardState, TransformComponent& t)
{
    const double rotationDelta{ RotationPerSecond / FrameTimeMsD };
    if (keyboardState[SDL_SCANCODE_A])
    {
        t.rotation -= rotationDelta;
    }
    if (keyboardState[SDL_SCANCODE_D])
    {
        t.rotation += rotationDelta;
    }

    constexpr double epsilon{std::numeric_limits<double>::epsilon()};
    while (t.rotation < 0.0 - epsilon)
    {
        t.rotation += FullCircleDegreesD;
    }
    while (t.rotation >= FullCircleDegreesD + epsilon)
    {
        t.rotation -= FullCircleDegreesD;
    }
}
