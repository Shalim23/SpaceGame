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

                return;
            }

            if (!outOfBoundsEntity.has_value())
            {
                createOutOfBoundsEntity(world);
            }
        });
}

bool WorldBoundsSystem::isPlayerInRange(const TransformComponent& playerTransform) const
{
    return fl::inRange(playerTransform.location.x, -boundsPixelSize_, boundsPixelSize_) &&
        fl::inRange(playerTransform.location.y, -boundsPixelSize_, boundsPixelSize_);
}

void WorldBoundsSystem::createOutOfBoundsEntity(World& world) const
{
    auto e{world.createEntity()};
    world.addComponent<OutOfWorldBoundsComponent>(e);
    auto& widget{world.addComponent<WidgetComponent>(e)};
    widget.setLayer(WidgetLayer::EFFECTS);

    auto& background{widget.addElement()};
    background.renderData.texture = renderSystem_->getTexture(TextureType::white_pixel).texture;
    
    const SDL_Point screenSize{renderSystem_->getScreenSize()};
    background.renderData.sourceRect.x = 0;
    background.renderData.sourceRect.y = 0;
    background.renderData.destinationRect.x = 0;
    background.renderData.destinationRect.y = 0;
    background.renderData.sourceRect.w = screenSize.x;
    background.renderData.destinationRect.w = screenSize.x;
    background.renderData.sourceRect.h = screenSize.y;
    background.renderData.destinationRect.h = screenSize.y;
    SDL_SetTextureColorMod(background.renderData.texture, 0, 0, 0);

    constexpr Uint64 backgroundAnimationTime{7000};
    background.animation.emplace(WidgetElementAnimation{backgroundAnimationTime,
    [&background](const float delta)
    {
        constexpr float minOpacity{0.0f};
        constexpr float maxOpacity{255.0f};
        const float currentOpacity{fl::lerp(minOpacity, maxOpacity, delta)};
        SDL_SetTextureAlphaMod(background.renderData.texture, static_cast<Uint8>(currentOpacity));
    }});
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
