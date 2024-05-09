#include "WorldBoundsSystem.h"
#include "../World.h"
#include "../SystemsManager.h"

void WorldBoundsSystem::init(World& w, SystemsManager& sm)
{
    m_render_system = &sm.getSystem<RenderSystem>();
    for (int i{ -m_grid_size }; i <= m_grid_size; ++i)
    {
        for (int k{ -m_grid_size }; k <= m_grid_size; ++k)
        {
            auto e{ w.createEntity() };

            auto& sprite{ w.addComponent<SpriteComponent>(e) };
            auto& texture{ m_render_system->getTexture(TextureType::Backgrounds_big_purple) };
            sprite.layer = SpriteLayer::BACKGROUND;
            sprite.render_data.texture = texture.texture;
            sprite.render_data.texture_size = texture.size;

            auto& transform{ w.addComponent<TransformComponent>(e) };
            transform.location.x = sprite.render_data.texture_size.x * i;
            transform.location.y = sprite.render_data.texture_size.y * k;
        }
    }
}

void WorldBoundsSystem::update(World& w)
{
    w.forEach<PlayerComponent>([this, &w](const Entity ent, PlayerComponent& comp)
        {
            const auto& player_transform{ *w.tryGetComponent<TransformComponent>(ent) };
            const std::optional<Entity> out_of_bounds_ent{getOutOfWorldBoundsComponentEntity(w)};
            if (isPlayerInRange(player_transform))
            {
                if (out_of_bounds_ent.has_value())
                {
                    w.destroyEntity(out_of_bounds_ent.value());
                }
            }
            else
            {
                /*if (!out_of_bounds_ent.has_value())
                {
                    createOutOfBoundsEntity(w, player_transform);
                    m_fade_in_start = SDL_GetTicks64();
                }
                else
                {
                    const Uint64 current_time_ms{SDL_GetTicks64()};
                    const Uint64 diff{ current_time_ms - m_fade_in_start };
                    if (diff < m_fade_in_time_ms)
                    {
                        auto& render_data{*w.tryGetComponent<UISpriteComponent>(out_of_bounds_ent.value())};
                        const float delta{static_cast<float>(diff) / m_fade_in_timef_ms };
                        constexpr float max_opacity{255.0f};
                        SDL_SetTextureAlphaMod(render_data.texture, static_cast<Uint8>(max_opacity * delta));
                    }
                }*/
            }
        });
}

bool WorldBoundsSystem::isPlayerInRange(const TransformComponent& player_transform) const
{
    return inRange(player_transform.location.x, -m_bounds_pixel_size, m_bounds_pixel_size) &&
        inRange(player_transform.location.y, -m_bounds_pixel_size, m_bounds_pixel_size);
}

bool WorldBoundsSystem::inRange(const float value, const float min_value, const float max_value) const
{
    return (value >= min_value) && (value <= max_value);
}

void WorldBoundsSystem::createOutOfBoundsEntity(World& w, const TransformComponent& player_transform)
{
    auto e{w.createEntity()};
    w.addComponent<OutOfWorldBoundsComponent>(e);

    /*auto& render_data{w.addComponent<UISpriteComponent>(e)};
    render_data.layer = UISpriteLayer::OUT_OF_WORLD_BOUNDS_EFFECT;
    render_data.texture = m_render_system->getTexture(TextureType::white_pixel).texture;
    
    const SDL_Point screen_size{m_render_system->getScreenSize()};
    render_data.src.x = render_data.src.y = render_data.dst.x = render_data.dst.y = 0;
    render_data.src.w = render_data.dst.w = screen_size.x;
    render_data.src.h = render_data.dst.h = screen_size.y;
    SDL_SetTextureColorMod(render_data.texture, 0, 0, 0);
    SDL_SetTextureAlphaMod(render_data.texture, 0);*/
}

std::optional<Entity> WorldBoundsSystem::getOutOfWorldBoundsComponentEntity(World& w) const
{
    std::optional<Entity> ent_opt;
    w.forEach<OutOfWorldBoundsComponent>([&ent_opt](const Entity ent, OutOfWorldBoundsComponent& comp)
    {
        ent_opt.emplace(ent);
    });

    return ent_opt;
}
