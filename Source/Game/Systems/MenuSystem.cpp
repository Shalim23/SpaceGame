#include "MenuSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include "../GameplayStatics.h"
#include "../Constants.h"

namespace
{
	constexpr size_t mainMenuBackgroundElementsCount{ 50 };
	constexpr SDL_FPoint mainMenuBackgroundElementsMinMaxScale{ .x = 1.0f, .y = 4.0f };
	constexpr SDL_Point mainMenuBackgroundElementsMinMaxAnimationTimeMs{ .x = 2000, .y = 7000 };
	constexpr std::array backgroundElementTextures{
        TextureType::Stars_star1, TextureType::Stars_star2, TextureType::Stars_star3 };
}

void MenuSystem::init(World& world, SystemsManager& systemsManager)
{
	renderSystem_ = &systemsManager.getSystem<RenderSystem>();
	randomSystem_ = &systemsManager.getSystem<RandomSystem>();
}

void MenuSystem::update(World& world)
{
	const GameStateType gameState{gameplayStatics::getCurrentGameState(world)};
	switch (gameState)
	{
	case GameStateType::MAIN_MENU:
	case GameStateType::INGAME_MENU:
	{
		if (WidgetComponent* component = getMenuWidget(world))
		{
			for (auto& widget : component->updateWidgets())
			{
				if (widget.hasAnimation() && widget.isAnimationFinished())
				{
					addMainMenuBackgroundElementAnimation(widget);
				}
			}
			//#TODO
			// track mouse click position
		}
		else
		{
			createMenuWidget(world, gameState);
		}
		
		break;
    }
	}
}

void MenuSystem::createMenuWidget(World& world, const GameStateType gameState) const
{
	switch (gameState)
	{
	case GameStateType::MAIN_MENU:
	{
		createMainMenu(world);
		break;
	}
	case GameStateType::INGAME_MENU:
	{
		createInGameMenu(world);
		break;
	}
	}
}

void MenuSystem::createMainMenu(World& world) const
{
    const Entity entity{ world.createEntity() };
    auto& widget{ world.addComponent<ComponentType::Widget>(entity) };
    widget.setLayer(WidgetLayer::MENU);

	createMainMenuBackground(entity, widget);
}

void MenuSystem::createMainMenuBackground(const Entity entity, WidgetComponent& widgetComponent) const
{
	const SDL_FPoint screenSize{renderSystem_->getScreenSizeF()};
	for (size_t i{ 0 }; i < mainMenuBackgroundElementsCount; ++i)
	{
		createMainMenuBackgroundElement(entity, widgetComponent, screenSize);
	}
}

void MenuSystem::createMainMenuBackgroundElement(const Entity entity, WidgetComponent& widgetComponent, const SDL_FPoint& screenSize) const
{
    const auto randomBackgroundElementTextureIndex{
		randomSystem_->getRandomValue(SDL_Point{.x = 0, .y = backgroundElementTextures.size()})};

	Widget& backgroundElement{ widgetComponent.addWidget() };
	auto& renderData{backgroundElement.updateRenderData()};
	const auto& textureInfo{ renderSystem_->createDynamicTexture(
		backgroundElementTextures[1], entity) };
	renderData.texture = textureInfo.texture;
	renderData.textureSize = textureInfo.size;
	renderData.sourceRect = functionsLibrary::makeRect(constants::sdlZeroPoint, renderData.textureSize);
	
    renderData.destinationRect = createMainMenuBackgroundElementDestRect(screenSize, renderData.textureSize);

    //const SDL_Color randomColor{ randomSystem_->getRandomColor() };
    //SDL_SetTextureColorMod(renderData.texture, randomColor.r, randomColor.g, randomColor.b);
	addMainMenuBackgroundElementAnimation(backgroundElement);
}

SDL_FRect MenuSystem::createMainMenuBackgroundElementDestRect(const SDL_FPoint& screenSize, const SDL_Point& textureSize) const
{
	const float elementRandomScale{ randomSystem_->getRandomValue(mainMenuBackgroundElementsMinMaxScale)};
    return functionsLibrary::makeRect(
        SDL_FPoint{
        .x = randomSystem_->getRandomValue(SDL_FPoint{.x = 0, .y = screenSize.x}),
        .y = randomSystem_->getRandomValue(SDL_FPoint{.x = 0, .y = screenSize.y})
        },
        SDL_FPoint{
        .x = textureSize.x * elementRandomScale,
        .y = textureSize.y * elementRandomScale
        });
}

void MenuSystem::addMainMenuBackgroundElementAnimation(Widget& widget) const
{
    const auto randomAnimationTime{ randomSystem_->getRandomValue(mainMenuBackgroundElementsMinMaxAnimationTimeMs) };
    widget.addAnimation(randomAnimationTime,
        [texture = widget.updateRenderData().texture](const float delta)
        {
            functionsLibrary::lerpOpacity(texture, delta);
        });
}

void MenuSystem::createInGameMenu(World& world) const
{
}

WidgetComponent* MenuSystem::getMenuWidget(World& world) const
{
    auto& widgets{ world.getComponents<ComponentType::Widget>() };
	auto iter{std::ranges::find_if(widgets, [](const auto& component)
		{ return component.instance.getLayer() == WidgetLayer::MENU; })};

	return iter == widgets.end() ? nullptr : &iter->instance;
}
