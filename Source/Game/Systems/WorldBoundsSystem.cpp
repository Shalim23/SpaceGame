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

            auto& render_comp{ w.addComponent<RenderComponent>(e) };
            auto& texture{ m_render_system->getTexture(TextureType::Backgrounds_big_purple) };
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
            const std::vector<Entity> out_of_bounds_ents{w.getEntities<OutOfWorldBoundsComponent>()};
            if (isPlayerInRange(player_transform))
            {
                if (out_of_bounds_ents.size() != 0)
                {
                    w.destroyEntity(*out_of_bounds_ents.begin());
                }
            }
            else
            {
                if (out_of_bounds_ents.size() == 0)
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
                        auto& render_comp{*w.tryGetComponent<UIRenderComponent>(*out_of_bounds_ents.begin())};
                        const float delta{static_cast<float>(diff) / m_fade_in_timef_ms };
                        constexpr float max_opacity{255.0f};
                        SDL_SetTextureAlphaMod(render_comp.texture, static_cast<Uint8>(max_opacity * delta));
                    }
                }
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

    auto& render_comp{w.addComponent<UIRenderComponent>(e)};
    render_comp.layer = UIRenderLayer::OUT_OF_WORLD_BOUNDS_EFFECT;
    render_comp.texture = m_render_system->getTexture(TextureType::white_pixel).texture;
    
    const SDL_Point screen_size{m_render_system->getScreenSize()};
    render_comp.src.x = render_comp.src.y = render_comp.dst.x = render_comp.dst.y = 0;
    render_comp.src.w = render_comp.dst.w = screen_size.x;
    render_comp.src.h = render_comp.dst.h = screen_size.y;
    SDL_SetTextureColorMod(render_comp.texture, 0, 0, 0);
    SDL_SetTextureAlphaMod(render_comp.texture, 0);
}
