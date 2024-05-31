#include "GameplaySystem.h"
#include "../World.h"
#include "../SystemsManager.h"

void GameplaySystem::init(World& world, SystemsManager& systemsManager)
{
    const auto entity{ world.createEntity() };
    auto& gameStateComponent{world.addComponent<GameStateComponent>(entity)};
#ifdef WIDGETBUILDER
    gameStateComponent.gameState = GameStateType::NONE;
#else
    gameStateComponent.gameState = GameStateType::MAIN_MENU;
#endif

    //createPlayer(world, systemsManager);
}

void GameplaySystem::createPlayer(World& world, SystemsManager& systemsManager) const
{
    auto& dbSystem{ systemsManager.getSystem<DatabaseSystem>() };

    const auto entity{ world.createEntity() };
    world.addComponents<PlayerComponent, MovementComponent, TransformComponent, GameObjectComponent>(entity);

    auto& render{ world.addComponent<RenderComponent>(entity) };
    const auto& texture{ dbSystem.getTexture(TextureType::PlayerShip_playerShip2_orange) };
    render.layer = RenderLayer::Player;
    render.data.texture = texture.texture;
    render.data.textureSize = texture.size;
}
