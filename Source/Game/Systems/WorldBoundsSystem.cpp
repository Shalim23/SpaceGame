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
    dbSystem_ = &systemsManager.getSystem<DataSystem>();
    const auto& renderSystem{systemsManager.getSystem<RenderSystem>()};
    screenSizeF_ = renderSystem.getScreenSizeF();

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
            for (const auto& oobComponent : outOfBoundsComponents)
            {
                world.queueEntityToDestoy(oobComponent.entity);
            }
        }

        return;
    }

    if (outOfBoundsComponents.size() == 0)
    {
        createOutOfBoundsWidget(world);
    }
}

void WorldBoundsSystem::generateBackground(World& world)
{
    for (int i{ -gridSize }; i <= gridSize; ++i)
    {
        for (int k{ -gridSize }; k <= gridSize; ++k)
        {
            auto e{ world.createEntity() };
            world.addComponent<GameObjectComponent>(e);

            auto& render{ world.addComponent<RenderComponent>(e) };
            auto& texture{ dbSystem_->getTexture(TextureType::Backgrounds_big_purple) };
            render.layer = RenderLayer::Background;
            render.data.texture = texture.texture;
            render.data.textureSize = texture.size;

            auto& transform{ world.addComponent<TransformComponent>(e) };
            transform.location.x = render.data.textureSize.x * i;
            transform.location.y = render.data.textureSize.y * k;
        }
    }
}

bool WorldBoundsSystem::isPlayerInRange(const TransformComponent& playerTransform) const
{
    return utils::inRange(playerTransform.location.x, minMaxBounds) &&
        utils::inRange(playerTransform.location.y, minMaxBounds);
}

void WorldBoundsSystem::createOutOfBoundsWidget(World& world) const
{
    createBackground(world);
    createText(world);
}

Entity WorldBoundsSystem::createOutOfBoundsEntity(World& world) const
{
    const Entity entity{ world.createEntity() };
    world.addComponents<OutOfWorldBoundsComponent, UIComponent>(entity);
    auto& render{ world.addComponent<RenderComponent>(entity) };
    render.layer = RenderLayer::UIEffects;

    return entity;
}

void WorldBoundsSystem::createBackground(World& world) const
{
    auto entity{ createOutOfBoundsEntity(world)};

    auto& renderData{world.tryGetComponent<RenderComponent>(entity)->data};
    const auto& info{dbSystem_->createDynamicTexture(TextureType::white_pixel, entity)};
    renderData.texture = info.texture;
    renderData.textureSize = info.size;

    renderData.sourceRect = utils::makeRect(constants::sdlZeroPoint, renderData.textureSize);
    renderData.destinationRect = utils::makeRect(screenSizeF_ / 2.0f, screenSizeF_);
    renderData.scale = screenSizeF_.x;

    constexpr SDL_Color blackColor{ .r = 0, .g = 0, .b = 0 };
    SDL_SetTextureColorMod(renderData.texture,
       blackColor.r, blackColor.g, blackColor.b);

    auto& animationComponent{world.addComponent<WidgetAnimationComponent>(entity)};
    animationComponent.totalAnimationTime = 7.0;
    animationComponent.animation = 
        [texture = renderData.texture](const float delta)
        {
            constexpr float minOpacity{ 0.0f };
            constexpr float maxOpacity{ 255.0f };
            const float currentOpacity{ utils::lerp(minOpacity, maxOpacity, delta) };
            SDL_SetTextureAlphaMod(texture, static_cast<Uint8>(currentOpacity));
        };
}

void WorldBoundsSystem::createText(World& world) const
{
    {
        auto entity{ createOutOfBoundsEntity(world) };
        auto& renderData{ world.tryGetComponent<RenderComponent>(entity)->data };
        
        const auto& info{ dbSystem_->getText(TextType::Warning)};
        renderData.texture = info.texture;
        renderData.textureSize = info.size;

        renderData.sourceRect = utils::makeRect(constants::sdlZeroPoint,
            renderData.textureSize);

        renderData.destinationRect = utils::makeRect(
            SDL_FPoint
            {
                .x = 1920.0f / 2.0f,
                .y = 780.0f
            },
            SDL_FPoint
            {
                .x = static_cast<float>(renderData.textureSize.x),
                .y = static_cast<float>(renderData.textureSize.y)
            }
            );
    }

    {
        auto entity{ createOutOfBoundsEntity(world) };
        auto& renderData{ world.tryGetComponent<RenderComponent>(entity)->data };

        const auto& info{ dbSystem_->getText(TextType::Highradiationlevel) };
        renderData.texture = info.texture;
        renderData.textureSize = info.size;

        renderData.sourceRect = utils::makeRect(constants::sdlZeroPoint,
            renderData.textureSize);

        renderData.destinationRect = utils::makeRect(
            SDL_FPoint
            {
                .x = 1920.0f / 2.0f,
                .y = 850.0f
            },
            SDL_FPoint
            {
                .x = static_cast<float>(renderData.textureSize.x),
                .y = static_cast<float>(renderData.textureSize.y)
            }
        );
    }
}

