#include "WorldBoundsSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include "../FunctionsLibrary.h"
#include "../Constants.h"

void WorldBoundsSystem::init(World& world, SystemsManager& systemsManager)
{
    renderSystem_ = &systemsManager.getSystem<RenderSystem>();
    textSystem_ = &systemsManager.getSystem<TextSystem>();
}

void WorldBoundsSystem::postInit(World& world, SystemsManager& systemsManager)
{
    generateBackground(world);
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

void WorldBoundsSystem::generateBackground(World& world)
{
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

bool WorldBoundsSystem::isPlayerInRange(const TransformComponent& playerTransform) const
{
    return fl::inRange(playerTransform.location.x, -boundsPixelSize_, boundsPixelSize_) &&
        fl::inRange(playerTransform.location.y, -boundsPixelSize_, boundsPixelSize_);
}

void WorldBoundsSystem::createOutOfBoundsEntity(World& world) const
{
    const Entity entity{world.createEntity()};
    world.addComponent<OutOfWorldBoundsComponent>(entity);
    auto& widget{world.addComponent<WidgetComponent>(entity)};
    widget.setLayer(WidgetLayer::EFFECTS);

    createBackgroundWidget(widget);
    createTextWidget(entity, widget);
}

void WorldBoundsSystem::createBackgroundWidget(WidgetComponent& widgetComponent) const
{
    Widget& background{ widgetComponent.addWidget() };

    RenderData& backgroundRenderData{ background.updateRenderData() };
    backgroundRenderData.texture = renderSystem_->getTexture(TextureType::white_pixel).texture;

    backgroundRenderData.sourceRect = fl::makeRect(constants::sdlZeroPoint, renderSystem_->getScreenSize());
    backgroundRenderData.destinationRect = fl::makeRect(constants::sdlZeroPointF, renderSystem_->getScreenSizeF());

    constexpr SDL_Color blackColor{ .r = 0, .g = 0, .b = 0 };
    SDL_SetTextureColorMod(backgroundRenderData.texture,
        blackColor.r, blackColor.g, blackColor.b);

    constexpr Uint64 backgroundAnimationTime{ 7000 };
    background.addAnimation(backgroundAnimationTime,
        [texture = backgroundRenderData.texture](const float delta)
        {
            constexpr float minOpacity{ 0.0f };
            constexpr float maxOpacity{ 255.0f };
            const float currentOpacity{ fl::lerp(minOpacity, maxOpacity, delta) };
            SDL_SetTextureAlphaMod(texture, static_cast<Uint8>(currentOpacity));
        });
}

void WorldBoundsSystem::createTextWidget(const Entity entity, WidgetComponent& widgetComponent) const
{
    const SDL_FPoint screenSize{ renderSystem_->getScreenSizeF() };
    const SDL_FPoint screenSizeModifier{renderSystem_->getScreenSizeF() / constants::baseScreenSize};

    float nextTextPosition{780.0f};

    {
        Widget& warningText{ widgetComponent.addWidget() };

        RenderData& warningTextRenderData{ warningText.updateRenderData() };
        warningTextRenderData = textSystem_->getText(TextType::Warning);

        warningTextRenderData.sourceRect = fl::makeRect(constants::sdlZeroPoint,
            warningTextRenderData.textureSize);

        warningTextRenderData.destinationRect = fl::makeRect(
            SDL_FPoint
            {
                .x = screenSize.x / 2.0f -
                    (warningTextRenderData.textureSize.x / 2.0f * screenSizeModifier.x),
                .y = nextTextPosition * screenSizeModifier.y
            },
            screenSizeModifier * warningTextRenderData.textureSize
            );

        nextTextPosition =
            warningTextRenderData.destinationRect.y  + warningTextRenderData.textureSize.y;
    }

    {
        Widget& radiationText{ widgetComponent.addWidget() };

        RenderData& radiationTextRenderData{ radiationText.updateRenderData() };
        radiationTextRenderData = textSystem_->getText(TextType::Highradiationlevel);

        radiationTextRenderData.sourceRect = fl::makeRect(constants::sdlZeroPoint,
            radiationTextRenderData.textureSize);

        radiationTextRenderData.destinationRect = fl::makeRect(
            SDL_FPoint
            {
                .x = screenSize.x / 2.0f -
                    (radiationTextRenderData.textureSize.x / 2.0f * screenSizeModifier.x),
                .y = nextTextPosition + (10.0f * screenSizeModifier.y)
            },
            screenSizeModifier * radiationTextRenderData.textureSize
        );
    }
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
