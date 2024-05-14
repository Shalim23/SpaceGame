#include "WorldBoundsSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include "../FunctionsLibrary.h"
#include "../GameplayStatics.h"
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
    const auto& playerComponent{gameplayStatics::getPlayerComponent(world)};
    const auto& playerTransform{ *world.tryGetComponent<ComponentType::Transform>(playerComponent.entity) };
    const auto& outOfBoundsComponents{ world.getComponents<ComponentType::OutOfWorldBounds>() };

    if (isPlayerInRange(playerTransform))
    {
        if (outOfBoundsComponents.size() > 0)
        {
            world.destroyEntity(outOfBoundsComponents.at(0).entity);
        }

        return;
    }

    if (outOfBoundsComponents.size() == 0)
    {
        createOutOfBoundsEntity(world);
    }
}

void WorldBoundsSystem::generateBackground(World& world)
{
    for (int i{ -gridSize_ }; i <= gridSize_; ++i)
    {
        for (int k{ -gridSize_ }; k <= gridSize_; ++k)
        {
            auto e{ world.createEntity() };

            auto& sprite{ world.addComponent<ComponentType::Sprite>(e) };
            auto& texture{ renderSystem_->getTexture(TextureType::Backgrounds_big_purple) };
            sprite.layer = SpriteLayer::BACKGROUND;
            sprite.renderData.texture = texture.texture;
            sprite.renderData.textureSize = texture.size;

            auto& transform{ world.addComponent<ComponentType::Transform>(e) };
            transform.location.x = sprite.renderData.textureSize.x * i;
            transform.location.y = sprite.renderData.textureSize.y * k;
        }
    }
}

bool WorldBoundsSystem::isPlayerInRange(const TransformComponent& playerTransform) const
{
    return functionsLibrary::inRange(playerTransform.location.x, -boundsPixelSize_, boundsPixelSize_) &&
        functionsLibrary::inRange(playerTransform.location.y, -boundsPixelSize_, boundsPixelSize_);
}

void WorldBoundsSystem::createOutOfBoundsEntity(World& world) const
{
    const Entity entity{world.createEntity()};
    world.addComponent<ComponentType::OutOfWorldBounds>(entity);
    auto& widget{world.addComponent<ComponentType::Widget>(entity)};
    widget.setLayer(WidgetLayer::EFFECTS);

    createBackgroundWidget(widget);
    createTextWidget(entity, widget);
}

void WorldBoundsSystem::createBackgroundWidget(WidgetComponent& widgetComponent) const
{
    Widget& background{ widgetComponent.addWidget() };

    RenderData& backgroundRenderData{ background.updateRenderData() };
    backgroundRenderData.texture = renderSystem_->getTexture(TextureType::white_pixel).texture;

    backgroundRenderData.sourceRect = functionsLibrary::makeRect(constants::sdlZeroPoint, renderSystem_->getScreenSize());
    backgroundRenderData.destinationRect = functionsLibrary::makeRect(constants::sdlZeroPointF, renderSystem_->getScreenSizeF());

    constexpr SDL_Color blackColor{ .r = 0, .g = 0, .b = 0 };
    SDL_SetTextureColorMod(backgroundRenderData.texture,
        blackColor.r, blackColor.g, blackColor.b);

    constexpr Uint64 backgroundAnimationTime{ 7000 };
    background.addAnimation(backgroundAnimationTime,
        [texture = backgroundRenderData.texture](const float delta)
        {
            constexpr float minOpacity{ 0.0f };
            constexpr float maxOpacity{ 255.0f };
            const float currentOpacity{ functionsLibrary::lerp(minOpacity, maxOpacity, delta) };
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

        warningTextRenderData.sourceRect = functionsLibrary::makeRect(constants::sdlZeroPoint,
            warningTextRenderData.textureSize);

        warningTextRenderData.destinationRect = functionsLibrary::makeRect(
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

        radiationTextRenderData.sourceRect = functionsLibrary::makeRect(constants::sdlZeroPoint,
            radiationTextRenderData.textureSize);

        radiationTextRenderData.destinationRect = functionsLibrary::makeRect(
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

