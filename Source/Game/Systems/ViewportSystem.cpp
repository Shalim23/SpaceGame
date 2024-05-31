#include "ViewportSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include "../Constants.h"

namespace
{
    constexpr SDL_FPoint baseScreenSize{ .x = 1920.0f, .y = 1080.0f };
}

void ViewportSystem::init(World& world, SystemsManager& systemsManager)
{
    renderSystem_ = &systemsManager.getSystem<RenderSystem>();
    screenSize_ = renderSystem_->getScreenSizeF();
    halfScreenSize_ = screenSize_ / 2.0f;
    screenSizeModifier_ = renderSystem_->getScreenSizeF() / baseScreenSize;

    renderDataByLayer_ = std::vector<std::vector<RenderData>>{ static_cast<size_t>(RenderLayer::Count) };
}

void ViewportSystem::update(World& world, const double deltaTime)
{
    for (auto& layer : renderDataByLayer_)
    {
        layer.clear();
    }

    for (auto& renderComponent : world.getComponents<RenderComponent>())
    {
        if (world.hasComponent<GameObjectComponent>(renderComponent.entity))
        {
            auto& component{ renderComponent.instance };
            auto renderData{setupGameObjectRenderData(world, renderComponent.entity, component.data)};
            if (renderData.has_value())
            {
                auto& layer{ renderDataByLayer_[static_cast<size_t>(component.layer)] };
                layer.emplace_back(renderData.value());
            }

            continue;
        }
        
        if (world.hasComponent<UIComponent>(renderComponent.entity))
        {
            auto& component{renderComponent.instance};
            auto& layer{ renderDataByLayer_[static_cast<size_t>(component.layer)]};
            layer.emplace_back(setupUIRenderData(component.data));

            continue;
        }

        assert(!"Unknown Render Component");
    }
}

const std::vector<std::vector<RenderData>>& ViewportSystem::getRenderData() const
{
    return renderDataByLayer_;
}

std::optional<RenderData> ViewportSystem::setupGameObjectRenderData(World& world,
    const Entity entity, const RenderData& sourceData)
{
    const auto& playerComponents{ world.getComponents<PlayerComponent>() };
    if (playerComponents.size() == 0)
    {
        return std::nullopt;
    }

    const auto& playerComponent{ world.getComponents<PlayerComponent>().front() };
    const Entity playerEntity{ playerComponent.entity };

    const auto& playerTransform{ *world.tryGetComponent<TransformComponent>(playerEntity) };
    
    const SDL_FRect cameraRect{
        .x = playerTransform.location.x - halfScreenSize_.x,
        .y = playerTransform.location.y - halfScreenSize_.y,
        .w = screenSize_.x,
        .h = screenSize_.y
    };

    if (entity == playerEntity)
    {
        RenderData dataToRender{sourceData};
        
        dataToRender.rotation = playerTransform.rotation;

        dataToRender.sourceRect = utils::makeRect(constants::sdlZeroPoint, dataToRender.textureSize);

        const SDL_Point halfTextureSize{ dataToRender.textureSize / 2};
        dataToRender.destinationRect = createDestinationRect(
            halfScreenSize_.x - halfTextureSize.x,
            halfScreenSize_.y - halfTextureSize.y,
            dataToRender.sourceRect.w,
            dataToRender.sourceRect.h
        );

        return std::optional{dataToRender};
    }

    const auto& renderTransform{ *world.tryGetComponent<TransformComponent>(entity) };
    const SDL_Point halfTextureSize{ sourceData.textureSize / 2 };
    const SDL_FRect renderRect{
        .x = renderTransform.location.x - halfTextureSize.x,
        .y = renderTransform.location.y - halfTextureSize.y,
        .w = static_cast<float>(sourceData.textureSize.x),
        .h = static_cast<float>(sourceData.textureSize.y)
    };

    SDL_FRect intersectRect{};
    if (!SDL_IntersectFRect(&cameraRect, &renderRect, &intersectRect))
    {
        return std::nullopt;
    }

    RenderData dataToRender{ sourceData };
    dataToRender.rotation = renderTransform.rotation;

    dataToRender.sourceRect = utils::makeRect(
        SDL_Point{
            .x = std::abs(static_cast<int>(renderRect.x - intersectRect.x)),
            .y = std::abs(static_cast<int>(renderRect.y - intersectRect.y))
        },
        SDL_Point{
            .x = static_cast<int>(intersectRect.w),
            .y = static_cast<int>(intersectRect.h)
        }
    );

    dataToRender.destinationRect = createDestinationRect(
        intersectRect.x + halfScreenSize_.x - playerTransform.location.x,
        intersectRect.y + halfScreenSize_.y - playerTransform.location.y,
        intersectRect.w, intersectRect.h
    );

    return std::optional{dataToRender};
}

RenderData ViewportSystem::setupUIRenderData(const RenderData& sourceData)
{
    RenderData dataToRender{ sourceData };
    
    const float scaledSizeX{ dataToRender.textureSize.x * screenSizeModifier_.x };
    const float scaledSizeY{ dataToRender.textureSize.y * screenSizeModifier_.y };
    dataToRender.destinationRect.x =
        dataToRender.destinationRect.x *
        screenSizeModifier_.x -
        (scaledSizeX / 2.0f * dataToRender.scale);

    dataToRender.destinationRect.y =
        dataToRender.destinationRect.y *
        screenSizeModifier_.y -
        (scaledSizeY / 2.0f * dataToRender.scale);

    dataToRender.destinationRect.w = dataToRender.textureSize.x
        * screenSizeModifier_.x * dataToRender.scale;
    dataToRender.destinationRect.h = dataToRender.textureSize.y
        * screenSizeModifier_.y * dataToRender.scale;

    return dataToRender;
}

SDL_FRect ViewportSystem::createDestinationRect(const float x, const float y, const float w, const float h)
{
    return SDL_FRect{ .x = roundf(x), .y = ceilf(y), .w = ceilf(w), .h = ceilf(h) };
}
