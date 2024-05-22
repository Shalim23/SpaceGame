#include "WorldBoundsSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include "../Utils.h"
#include "../GameplayStatics.h"
#include "../Constants.h"

namespace
{
    constexpr float boundsPixelSize{ 3500.0f };
    constexpr SDL_FPoint minMaxBounds{ .x = -boundsPixelSize, .y = boundsPixelSize };
    constexpr int gridSize{ 3 };
}

void WorldBoundsSystem::init(World& world, SystemsManager& systemsManager)
{
    dbSystem_ = &systemsManager.getSystem<DatabaseSystem>();
    generateBackground(world);
}

void WorldBoundsSystem::update(World& world, const double deltaTime)
{
    const GameStateType gameState{ gameplayStatics::getCurrentGameState(world) };
    if (gameState != GameStateType::INGAME)
    {
        return;
    }
    
    const auto& playerComponent{gameplayStatics::getPlayerComponent(world)};
    const auto& playerTransform{ *world.tryGetComponent<TransformComponent>(playerComponent.entity) };
    const auto& outOfBoundsComponents{ world.getComponents<OutOfWorldBoundsComponent>() };

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
    for (int i{ -gridSize }; i <= gridSize; ++i)
    {
        for (int k{ -gridSize }; k <= gridSize; ++k)
        {
            auto e{ world.createEntity() };

            auto& sprite{ world.addComponent<SpriteComponent>(e) };
            auto& texture{ dbSystem_->getTexture(TextureType::Backgrounds_big_purple) };
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
    return utils::inRange(playerTransform.location.x, minMaxBounds) &&
        utils::inRange(playerTransform.location.y, minMaxBounds);
}

void WorldBoundsSystem::createOutOfBoundsEntity(World& world) const
{
    const Entity entity{world.createEntity()};
    world.addComponent<OutOfWorldBoundsComponent>(entity);
    //auto& widget{world.addComponent<WidgetComponent>(entity)};
    //widget.setLayer(WidgetLayer::EFFECTS);

    //createBackgroundWidget(widget);
    //createTextWidget(entity, widget);
}

void WorldBoundsSystem::createBackgroundWidget(WidgetComponent& widgetComponent) const
{
    Widget& background{ widgetComponent.addWidget() };

    RenderData& backgroundRenderData{ background.updateRenderData() };
    backgroundRenderData.texture = dbSystem_->getTexture(TextureType::white_pixel).texture;

    //#TODO
    //backgroundRenderData.sourceRect = utils::makeRect(constants::sdlZeroPoint, renderSystem_->getScreenSize());
    //backgroundRenderData.destinationRect = utils::makeRect(constants::sdlZeroPointF, renderSystem_->getScreenSizeF());

    constexpr SDL_Color blackColor{ .r = 0, .g = 0, .b = 0 };
    SDL_SetTextureColorMod(backgroundRenderData.texture,
        blackColor.r, blackColor.g, blackColor.b);

    constexpr Uint64 backgroundAnimationTime{ 7000 };
    background.addAnimation(backgroundAnimationTime,
        [texture = backgroundRenderData.texture](const float delta)
        {
            utils::lerpOpacity(texture, delta);
        });
}

void WorldBoundsSystem::createTextWidget(const Entity entity, WidgetComponent& widgetComponent) const
{
    //#TODO
    
    /*const SDL_FPoint screenSize{ renderSystem_->getScreenSizeF() };
    const SDL_FPoint screenSizeModifier{renderSystem_->getScreenSizeF() / constants::baseScreenSize};

    float nextTextPosition{780.0f};

    {
        Widget& warningText{ widgetComponent.addWidget() };

        RenderData& warningTextRenderData{ warningText.updateRenderData() };
        warningTextRenderData = textSystem_->getText(TextType::Warning);

        warningTextRenderData.sourceRect = utils::makeRect(constants::sdlZeroPoint,
            warningTextRenderData.textureSize);

        warningTextRenderData.destinationRect = utils::makeRect(
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

        radiationTextRenderData.sourceRect = utils::makeRect(constants::sdlZeroPoint,
            radiationTextRenderData.textureSize);

        radiationTextRenderData.destinationRect = utils::makeRect(
            SDL_FPoint
            {
                .x = screenSize.x / 2.0f -
                    (radiationTextRenderData.textureSize.x / 2.0f * screenSizeModifier.x),
                .y = nextTextPosition + (10.0f * screenSizeModifier.y)
            },
            screenSizeModifier * radiationTextRenderData.textureSize
        );
    }*/
}

