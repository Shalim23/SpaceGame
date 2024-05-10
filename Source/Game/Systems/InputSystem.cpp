#include "InputSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include "../Constants.h"
#include <numbers>

void InputSystem::update(World& world)
{
    int len;
    const Uint8* const keyboardState{SDL_GetKeyboardState(&len)};

    world.forEach<PlayerComponent>([this, &world, &keyboardState]
        (const Entity entity, PlayerComponent&)
        {
            auto& transform{ *world.tryGetComponent<TransformComponent>(entity) };
            processRotation(keyboardState, transform);

            auto& movement{ *world.tryGetComponent<MovementComponent>(entity) };
            movement.forwardVector = calculateForwardVector(transform.rotation);
            movement.speedPerSecond =
                keyboardState[SDL_SCANCODE_W] ?
                movementSpeedPerSecond_ : 0.0f;
        });
}

void InputSystem::processRotation(const Uint8* const keyboardState, TransformComponent& transform)
{
    const double rotationDelta{ rotationRatePerSecond_ / constants::frameTimeMsD };
    if (keyboardState[SDL_SCANCODE_A])
    {
        transform.rotation -= rotationDelta;
    }
    if (keyboardState[SDL_SCANCODE_D])
    {
        transform.rotation += rotationDelta;
    }

    constexpr double epsilon{std::numeric_limits<double>::epsilon()};
    while (transform.rotation < 0.0 - epsilon)
    {
        transform.rotation += constants::fullCircleDegreesD;
    }
    while (transform.rotation >= constants::fullCircleDegreesD + epsilon)
    {
        transform.rotation -= constants::fullCircleDegreesD;
    }
}

SDL_FPoint InputSystem::calculateForwardVector(const double rotation) const
{
    const float radians{static_cast<float>(rotation * std::numbers::pi
        / (constants::fullCircleDegreesD / 2.0))};
    return SDL_FPoint{.x = sin(radians), .y = cos(radians) * -1.0f };
}
