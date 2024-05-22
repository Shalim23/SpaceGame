#include "ViewportSystem.h"
#include "../World.h"
#include "../SystemsManager.h"

namespace
{
    constexpr SDL_FPoint baseScreenSize{ .x = 1920.0f, .y = 1080.0f };
}

void ViewportSystem::init(World& world, SystemsManager& systemsManager)
{
    renderSystem_ = &systemsManager.getSystem<RenderSystem>();
}

void ViewportSystem::update(World& world, const double deltaTime)
{
    renderData_.clear();
    
    prepareGameObjectsRenderData(world);
    prepareUIRenderData(world);
}

const std::vector<RenderData>& ViewportSystem::getRenderData() const
{
    return renderData_;
}

void ViewportSystem::prepareGameObjectsRenderData(World& world)
{

}

void ViewportSystem::prepareUIRenderData(World& world)
{
    const SDL_FPoint screenSize{ renderSystem_->getScreenSizeF() };
    const SDL_FPoint screenSizeModifier{ renderSystem_->getScreenSizeF() / baseScreenSize };
    
    std::vector<WidgetComponent*> uiToRender{static_cast<size_t>(WidgetLayer::COUNT)};

    for (auto& widgetComponent : world.getComponents<WidgetComponent>())
    {
        auto& layerWidgetComponent{ uiToRender[static_cast<size_t>(widgetComponent.instance.getLayer())] };
        assert(!layerWidgetComponent);
        layerWidgetComponent = &widgetComponent.instance;
    }

    for (const auto& layer : uiToRender)
    {
        if (!layer)
        {
            continue;
        }

        for (const auto& renderData : layer->gatherRenderData())
        {
            auto& data { renderData_.emplace_back(*renderData) };
            
            const float scaledSizeX{ data.textureSize.x * screenSizeModifier.x };
            const float scaledSizeY{ data.textureSize.y * screenSizeModifier.y };
            data.destinationRect.x =
                renderData->destinationRect.x *
                screenSizeModifier.x -
                (scaledSizeX / 2.0f * data.scale);

            data.destinationRect.y =
                renderData->destinationRect.y *
                screenSizeModifier.y -
                (scaledSizeY / 2.0f * data.scale);

            data.destinationRect.w = data.textureSize.x * screenSizeModifier.x * data.scale;
            data.destinationRect.h = data.textureSize.y * screenSizeModifier.y * data.scale;
        }
    }
}
