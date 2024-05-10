#include "GameplaySystem.h"
#include "../World.h"
#include "../SystemsManager.h"

void GameplaySystem::init(World& w, SystemsManager& sm)
{
    createPlayer(w, sm);
}

void GameplaySystem::createPlayer(World& w, SystemsManager& sm) const
{
    auto& render_system{ sm.getSystem<RenderSystem>() };

    auto e{ w.createEntity() };
    w.addComponent<PlayerComponent>(e);
    w.addComponent<MovementComponent>(e);
    w.addComponent<TransformComponent>(e);

    auto& sprite{ w.addComponent<SpriteComponent>(e) };
    auto& texture{ render_system.getTexture(TextureType::PlayerShip_playerShip2_orange) };
    sprite.layer = SpriteLayer::PLAYER;
    sprite.render_data.texture = texture.texture;
    sprite.render_data.texture_size = texture.size;
}
