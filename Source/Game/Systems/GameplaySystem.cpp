#include "GameplaySystem.h"
#include "../World.h"
#include "../SystemsManager.h"

void GameplaySystem::postInit(World& world, SystemsManager& systemsManager)
{
    //const auto entity{ world.createEntity() };
    //world.addComponent<GameStateComponent>(entity);
    createPlayer(world, systemsManager);
}

void GameplaySystem::createPlayer(World& world, SystemsManager& systemsManager) const
{
    auto& renderSystem{ systemsManager.getSystem<RenderSystem>() };

    const auto entity{ world.createEntity() };
    world.addComponent<PlayerComponent>(entity);
    world.addComponent<MovementComponent>(entity);
    world.addComponent<TransformComponent>(entity);

    auto& sprite{ world.addComponent<SpriteComponent>(entity) };
    const auto& texture{ renderSystem.getTexture(TextureType::PlayerShip_playerShip2_orange) };
    sprite.layer = SpriteLayer::PLAYER;
    sprite.renderData.texture = texture.texture;
    sprite.renderData.textureSize = texture.size;
}
