#pragma once
#include "World.h"

namespace gameplayStatics
{
    static Component<PlayerComponent>& getPlayerComponent(World& world)
    {
        return world.getComponents<ComponentType::Player>().at(0);
    }

    static GameStateType getCurrentGameState(World& world)
    {
        return world.getComponents<ComponentType::GameState>().at(0).instance.gameState;
    }

    static void clearWidgetDynamicTextures(std::vector<DynamicTexture>& textures, World& world)
    {
        const auto& widgetComponents{ world.getComponents<ComponentType::Widget>() };

        std::vector<const DynamicTexture*> dynamicTextsToRemove;
        for (const auto& texture : textures)
        {
            if (!utils::containsIf(widgetComponents, [&texture](const auto& component)
                {   return texture.entity == component.entity; }))
            {
                dynamicTextsToRemove.push_back(&texture);
            }
        }

        for (const auto* textsToRemove : dynamicTextsToRemove)
        {
            for (SDL_Texture* texture : textsToRemove->textures)
            {
                SDL_DestroyTexture(texture);
            }

            std::erase_if(textures, [textsToRemove](const DynamicTexture& texts)
                {
                    return textsToRemove->entity == texts.entity;
                });
        }
    }
}
