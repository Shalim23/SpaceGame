#include "MovementSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include "../Constants.h"

void MovementSystem::update(World& world)
{
    for (auto& movement : world.getComponents<ComponentType::Movement>())
    {
        auto& movementComponent{movement.instance};
        auto& transform{ *world.tryGetComponent<ComponentType::Transform>(movement.entity) };
        const float movementDelta{ movementComponent.speedPerSecond / constants::frameTimeMsF };
        transform.location.x += movementComponent.forwardVector.x * movementDelta;
        transform.location.y += movementComponent.forwardVector.y * movementDelta;
    }
}
