#include "InputSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include "../Constants.h"
#include <numbers>

void InputSystem::update(World& w)
{
    int len;
    const Uint8* keyboardState{SDL_GetKeyboardState(&len)};

    w.forEach<InputComponent>([this, &w, &keyboardState]
        (const Entity ent, InputComponent& comp)
        {
            auto& transform{ *w.tryGetComponent<TransformComponent>(ent) };
            auto& movement{ *w.tryGetComponent<MovementComponent>(ent) };
            processRotation(keyboardState, transform);
            movement.forward_vector = calculateForwardVector(transform.rotation);

            if (keyboardState[SDL_SCANCODE_W])
            {
                const float movementDelta{ MovementPerSecond / FrameTimeMsF };
                transform.location.x += movement.forward_vector.x * movementDelta;
                transform.location.y += movement.forward_vector.y * movementDelta;
            }
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

SDL_FPoint InputSystem::calculateForwardVector(const double rotation) const
{
    const float radians{static_cast<float>(rotation * std::numbers::pi / 180.0)};
    return SDL_FPoint{.x = sin(radians), .y = cos(radians) * -1.0f };
}
