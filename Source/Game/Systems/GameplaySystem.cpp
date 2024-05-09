#include "GameplaySystem.h"
#include "../World.h"
#include "../SystemsManager.h"

void GameplaySystem::init(World& w, SystemsManager& sm)
{
    auto& render_system{sm.getSystem<RenderSystem>()};
    
    auto e{ w.createEntity() };
    w.addComponent<PlayerComponent>(e);
    w.addComponent<MovementComponent>(e);
    w.addComponent<TransformComponent>(e);

    auto& render_comp{w.addComponent<SpriteComponent>(e)};
    auto& texture{ render_system.getTexture(TextureType::PlayerShip_playerShip2_orange)};
    render_comp.layer = SpriteLayer::PLAYER;
    render_comp.texture = texture.texture;
    render_comp.texture_size = texture.size;
}
