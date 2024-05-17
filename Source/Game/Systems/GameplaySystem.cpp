#include "GameplaySystem.h"
#include "../World.h"
#include "../SystemsManager.h"

void GameplaySystem::init(World& world, SystemsManager& systemsManager)
{
    const auto entity{ world.createEntity() };
    auto& gameStateComponent{world.addComponent<ComponentType::GameState>(entity)};
#ifdef WIDGETBUILDER
    gameStateComponent.gameState = GameStateType::NONE;
#else
    gameStateComponent.gameState = GameStateType::MAIN_MENU;
#endif

    //createPlayer(world, systemsManager);
}

void GameplaySystem::createPlayer(World& world, SystemsManager& systemsManager) const
{
    auto& renderSystem{ systemsManager.getSystem<RenderSystem>() };

    const auto entity{ world.createEntity() };
    world.addComponent<ComponentType::Player>(entity);
    world.addComponent<ComponentType::Movement>(entity);
    world.addComponent<ComponentType::Transform>(entity);

    auto& sprite{ world.addComponent<ComponentType::Sprite>(entity) };
    const auto& texture{ renderSystem.getTexture(TextureType::PlayerShip_playerShip2_orange) };
    sprite.layer = SpriteLayer::PLAYER;
    sprite.renderData.texture = texture.texture;
    sprite.renderData.textureSize = texture.size;
}
