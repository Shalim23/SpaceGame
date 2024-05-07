#include "GameplaySystem.h"
#include "../World.h"
#include "../SystemsManager.h"

void GameplaySystem::init(World& w, SystemsManager& sm)
{
    generateBackground(w, sm);
    
    auto& render_system{sm.getSystem<RenderSystem>()};
    
    auto e{ w.createEntity() };
    w.addComponent<CameraComponent>(e);
    w.addComponent<InputComponent>(e);
    w.addComponent<MovementComponent>(e);
    w.addComponent<TransformComponent>(e);

    auto& render_comp{w.addComponent<RenderComponent>(e)};
    auto& texture{ render_system.getTexture(TextureType::PlayerShip_playerShip2_orange)};
    render_comp.layer = RenderLayer::PLAYER;
    render_comp.texture = texture.texture;
    render_comp.texture_size = texture.size;
}

void GameplaySystem::generateBackground(World& w, SystemsManager& sm)
{
    auto& render_system{ sm.getSystem<RenderSystem>() };
    
    for (int i{ -5 }; i <= 5; ++i)
    {
        for (int k{ -5 }; k <= 5; ++k)
        {
            auto e{ w.createEntity() };

            auto& render_comp{ w.addComponent<RenderComponent>(e) };
            auto& texture{ render_system.getTexture(TextureType::Backgrounds_purple) };
            render_comp.layer = RenderLayer::BACKGROUND;
            render_comp.texture = texture.texture;
            render_comp.texture_size = texture.size;

            auto& transform{ w.addComponent<TransformComponent>(e) };
            transform.location.x = render_comp.texture_size.x * i;
            transform.location.y = render_comp.texture_size.y * k;
        }
    }
}
