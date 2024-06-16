#include "MovementSystem.h"
#include "../World.h"
#include "../SystemsManager.h"

void MovementSystem::update(World& world, const double deltaTime)
{
    for (auto& movement : world.getComponents<MovementComponent>())
    {
        auto& movementComponent{movement.instance};
        auto& transform{ *world.tryGetComponent<TransformComponent>(movement.entity) };
        const float movementDelta{ movementComponent.speedPerSecond * static_cast<float>(deltaTime) };
        transform.location.x += movementComponent.forwardVector.x * movementDelta;
        transform.location.y += movementComponent.forwardVector.y * movementDelta;
    }
}
