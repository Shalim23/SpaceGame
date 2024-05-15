#include "InputSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include "../Constants.h"
#include "../GameplayStatics.h"
#include <numbers>

namespace
{
    constexpr double rotationRatePerSecond{ 20.0 };
    constexpr float movementSpeedPerSecond{ 50.0 };
}

void InputSystem::update(World& world)
{
    const GameStateType gameState{ gameplayStatics::getCurrentGameState(world) };
    if (gameState != GameStateType::INGAME)
    {
        return;
    }

    int len;
    const Uint8* const keyboardState{SDL_GetKeyboardState(&len)};

    
    const auto& playerComponent{gameplayStatics::getPlayerComponent(world)};
    auto& transform{ *world.tryGetComponent<ComponentType::Transform>(playerComponent.entity) };
    processRotation(keyboardState, transform);

    auto& movement{ *world.tryGetComponent<ComponentType::Movement>(playerComponent.entity) };
    movement.forwardVector = calculateForwardVector(transform.rotation);
    movement.speedPerSecond = keyboardState[SDL_SCANCODE_W] ? movementSpeedPerSecond : 0.0f;
}

void InputSystem::processRotation(const Uint8* const keyboardState, TransformComponent& transform)
{
    const double rotationDelta{ rotationRatePerSecond / constants::frameTimeMsD };
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
