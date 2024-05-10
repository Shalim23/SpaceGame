#include "WorldBoundsSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include "../FunctionsLibrary.h"

void WorldBoundsSystem::postInit(World& world, SystemsManager& systemsManager)
{
    renderSystem_ = &systemsManager.getSystem<RenderSystem>();
    for (int i{ -gridSize_ }; i <= gridSize_; ++i)
    {
        for (int k{ -gridSize_ }; k <= gridSize_; ++k)
        {
            auto e{ world.createEntity() };

            auto& sprite{ world.addComponent<SpriteComponent>(e) };
            auto& texture{ renderSystem_->getTexture(TextureType::Backgrounds_big_purple) };
            sprite.layer = SpriteLayer::BACKGROUND;
            sprite.renderData.texture = texture.texture;
            sprite.renderData.textureSize = texture.size;

            auto& transform{ world.addComponent<TransformComponent>(e) };
            transform.location.x = sprite.renderData.textureSize.x * i;
            transform.location.y = sprite.renderData.textureSize.y * k;
        }
    }
}

void WorldBoundsSystem::update(World& world)
{
    world.forEach<PlayerComponent>([this, &world](const Entity entity, PlayerComponent&)
        {
            const auto& playerTransform{ *world.tryGetComponent<TransformComponent>(entity) };
            const std::optional<Entity> outOfBoundsEntity{getOutOfWorldBoundsComponentEntity(world)};
            if (isPlayerInRange(playerTransform))
            {
                if (outOfBoundsEntity.has_value())
                {
                    world.destroyEntity(outOfBoundsEntity.value());
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

bool WorldBoundsSystem::isPlayerInRange(const TransformComponent& playerTransform) const
{
    return fl::inRange(playerTransform.location.x, -boundsPixelSize_, boundsPixelSize_) &&
        fl::inRange(playerTransform.location.y, -boundsPixelSize_, boundsPixelSize_);
}

void WorldBoundsSystem::createOutOfBoundsEntity(World& world, const TransformComponent& playerTransform)
{
    auto e{world.createEntity()};
    world.addComponent<OutOfWorldBoundsComponent>(e);

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

std::optional<Entity> WorldBoundsSystem::getOutOfWorldBoundsComponentEntity(World& world) const
{
    std::optional<Entity> outOfBoundsEntity;
    world.forEach<OutOfWorldBoundsComponent>([&outOfBoundsEntity](const Entity entity, OutOfWorldBoundsComponent&)
    {
        outOfBoundsEntity.emplace(entity);
    });

    return outOfBoundsEntity;
}
