#include "GameplaySystem.h"
#include "../World.h"
#include "../SystemsManager.h"

void GameplaySystem::init(World& w, SystemsManager& sm)
{
    auto render_system{sm.getSystem<RenderSystem>()};
    
    auto e{ w.createEntity() };
    w.addComponent<CameraComponent>(e);
    w.addComponent<TransformComponent>(e);

    auto& render_comp{w.addComponent<RenderComponent>(e)};
    auto& texture{ render_system.getTexture(TextureType::PlayerShip_playerShip1_blue)};
    render_comp.layer = RenderLayer::PLAYER;
    render_comp.texture = texture.texture;
    render_comp.texture_size = texture.size;
}
