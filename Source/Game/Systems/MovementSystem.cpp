#include "MovementSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include "../Constants.h"

void MovementSystem::update(World& world)
{
    world.forEach<MovementComponent>(
        [&world](const Entity entity, MovementComponent& movementComponent)
        {
            auto& transform{ *world.tryGetComponent<TransformComponent>(entity) };
            const float movementDelta{ movementComponent.speedPerSecond / constants::frameTimeMsF };
            transform.location.x += movementComponent.forwardVector.x * movementDelta;
            transform.location.y += movementComponent.forwardVector.y * movementDelta;
        });
}
