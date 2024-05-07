#include "WorldBoundsSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include <iostream>

void WorldBoundsSystem::init(World& w, SystemsManager& sm)
{
    auto& render_system{ sm.getSystem<RenderSystem>() };
    for (int i{ -m_grid_size }; i <= m_grid_size; ++i)
    {
        for (int k{ -m_grid_size }; k <= m_grid_size; ++k)
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

void WorldBoundsSystem::update(World& w)
{
    w.forEach<PlayerComponent>([this, &w](const Entity ent, PlayerComponent& comp)
        {
            const auto& player_transform{ *w.tryGetComponent<TransformComponent>(ent) };
            if (!inRange(player_transform.location.x, -m_bounds_pixel_size, m_bounds_pixel_size) ||
                !inRange(player_transform.location.y, -m_bounds_pixel_size, m_bounds_pixel_size))
            {
                std::cout << "Out of World bounds!\n";
            }
        });
}

bool WorldBoundsSystem::inRange(const float value, const float min_value, const float max_value) const
{
    return (value >= min_value) && (value <= max_value);
}
