#include "MenuSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include "../GameplayStatics.h"
#include "../Constants.h"

namespace
{
	//constexpr size_t mainMenuBackgroundElementsCount{ 20 };
	//constexpr SDL_FPoint mainMenuBackgroundElementsMinMaxScale{ .x = 1.0f, .y = 2.0f };
	//constexpr SDL_Point mainMenuBackgroundElementsMinMaxAnimationTimeMs{ .x = 8000, .y = 12000 };
	//constexpr std::array backgroundElementTextures{
        //TextureType::Stars_star1, TextureType::Stars_star2, TextureType::Stars_star3 };
	constexpr Uint64 titleAnimationTimeMs{1000};
}

void MenuSystem::init(World& world, SystemsManager& systemsManager)
{
	renderSystem_ = &systemsManager.getSystem<RenderSystem>();
	randomSystem_ = &systemsManager.getSystem<RandomSystem>();
	textSystem_ = &systemsManager.getSystem<TextSystem>();
}

void MenuSystem::update(World& world, const double deltaTime)
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
				if (widget->hasAnimation() && widget->isAnimationFinished())
				{
					
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

	createTitle(widget);

    //createButton(widget, 700.0f, 0.0f, TextType::Play);
    //createButton(widget, 850.0f, 150.0f, TextType::Exit);

	//createMainMenuBackground(entity, widget);
}

void MenuSystem::createTitle(WidgetComponent& widgetComponent) const
{
    const SDL_FPoint screenSize{ renderSystem_->getScreenSizeF() };
    const SDL_FPoint screenSizeModifier{ renderSystem_->getScreenSizeF() / constants::baseScreenSize };
	constexpr float scale{4.0f};
	float nextTextPositionY{250.0f};

    {
		Widget& spaceText{ widgetComponent.addWidget() };

        RenderData& spaceTextRenderData{ spaceText.updateRenderData() };
        spaceTextRenderData = textSystem_->getText(TextType::SPACE);

        spaceTextRenderData.sourceRect = utils::makeRect(constants::sdlZeroPoint,
            spaceTextRenderData.textureSize);

   //     const float animationStartPosition{-spaceTextRenderData.textureSize.x
			//* screenSizeModifier.x * textScale };
   //     const float animationEndPosition{600.0f * screenSizeModifier.x};
   //     spaceTextRenderData.destinationRect = utils::makeRect(
   //         SDL_FPoint
   //         {
   //             .x = animationStartPosition,
   //             .y = nextTextPositionY * screenSizeModifier.y
   //         },
   //         screenSizeModifier * spaceTextRenderData.textureSize * textScale
   //     );

        const float scaledSizeX{ spaceTextRenderData.textureSize.x * screenSizeModifier.x };
        const float scaledSizeY{ spaceTextRenderData.textureSize.y * screenSizeModifier.y };
        spaceTextRenderData.destinationRect.x =
            750.0f * screenSizeModifier.x - (scaledSizeX / 2.0f * scale);

        spaceTextRenderData.destinationRect.y =
            350.0f * screenSizeModifier.y - (scaledSizeY / 2.0f * scale);

        spaceTextRenderData.destinationRect.w = spaceTextRenderData.textureSize.x * screenSizeModifier.x * scale;
        spaceTextRenderData.destinationRect.h = spaceTextRenderData.textureSize.y * screenSizeModifier.y * scale;

		/*nextTextPositionY =
            spaceTextRenderData.destinationRect.y + spaceTextRenderData.textureSize.y;*/

		/*spaceText.addAnimation(titleAnimationTimeMs,
			[&destRect = spaceTextRenderData.destinationRect,
			animationStartPosition, animationEndPosition
			](const float delta)
			{
				const float newPositionX{utils::lerp(
					animationStartPosition, animationEndPosition, delta)};
				destRect.x = newPositionX;
			}
		);*/
    }

    {
        Widget& gameText{ widgetComponent.addWidget() };

        RenderData& gameTextRenderData{ gameText.updateRenderData() };
        gameTextRenderData = textSystem_->getText(TextType::GAME);

        gameTextRenderData.sourceRect = utils::makeRect(constants::sdlZeroPoint,
            gameTextRenderData.textureSize);

        //const float animationStartPosition{ screenSize.x };
        //const float animationEndPosition{ 900.0f * screenSizeModifier.x };
        //gameTextRenderData.destinationRect = utils::makeRect(
        //    SDL_FPoint
        //    {
        //        .x = animationStartPosition,
        //        .y = nextTextPositionY + (90.0f * screenSizeModifier.y)
        //    },
        //    screenSizeModifier * gameTextRenderData.textureSize * scale
        //);

        const float scaledSizeX{ gameTextRenderData.textureSize.x * screenSizeModifier.x };
        const float scaledSizeY{ gameTextRenderData.textureSize.y * screenSizeModifier.y };
        gameTextRenderData.destinationRect.x =
            1000.0f * screenSizeModifier.x - (scaledSizeX / 2.0f * scale);

        gameTextRenderData.destinationRect.y =
            (350.0f + 150.0f) * screenSizeModifier.y - (scaledSizeY / 2.0f * scale);

        gameTextRenderData.destinationRect.w = gameTextRenderData.textureSize.x * screenSizeModifier.x * scale;
        gameTextRenderData.destinationRect.h = gameTextRenderData.textureSize.y * screenSizeModifier.y * scale;

        /*gameText.addAnimation(titleAnimationTimeMs,
            [&destRect = gameTextRenderData.destinationRect,
            animationStartPosition, animationEndPosition
            ](const float delta)
            {
                const float newPositionX{ utils::lerp(
                    animationStartPosition, animationEndPosition, delta) };
                destRect.x = newPositionX;
            }
        );*/
    }
}

void MenuSystem::createButton(WidgetComponent& widgetComponent,
    const float endPosition, const float padding, const TextType text) const
{
    const SDL_FPoint screenSize{ renderSystem_->getScreenSizeF() };
    const SDL_FPoint screenSizeModifier{ renderSystem_->getScreenSizeF() / constants::baseScreenSize };
    constexpr float scale{2.0f};

    {
        Widget& button{ widgetComponent.addWidget() };

        RenderData& buttonRenderData{ button.updateRenderData() };
        const auto& textureInfo{ renderSystem_->getTexture(TextureType::UI_buttonBlue)};
        buttonRenderData.texture = textureInfo.texture;
        buttonRenderData.textureSize = textureInfo.size;

        buttonRenderData.sourceRect = utils::makeRect(constants::sdlZeroPoint,
            buttonRenderData.textureSize);

        //const float animationStartPosition{ screenSize.y + padding };
        //auto scaled = buttonRenderData.textureSize.x * scale;
        //auto scaleDiffX = buttonRenderData.textureSize.x * scale * screenSizeModifier.x - buttonRenderData.textureSize.x * screenSizeModifier.x;
        //auto scaleDiffY = buttonRenderData.textureSize.y * scale - buttonRenderData.textureSize.y;
        //const float animationEndPosition{ endPosition * screenSizeModifier.y - scaleDiffY / 2.0f };
        /*buttonRenderData.destinationRect = utils::makeRect(
            SDL_FPoint
            {
                .x = screenSize.x / 2.0f,
                .y = screenSize.y / 2.0f
            },
            screenSizeModifier * buttonRenderData.textureSize * scale
        );*/

        const float scaledSizeX{ buttonRenderData.textureSize.x * screenSizeModifier.x};
        const float scaledSizeY{ buttonRenderData.textureSize.y * screenSizeModifier.y};
        const float animationEndPosition{700.0f};
        buttonRenderData.destinationRect.x =
            screenSize.x / 2.0f * screenSizeModifier.x - (scaledSizeX / 2.0f * scale);

        buttonRenderData.destinationRect.y =
            700.0f * screenSizeModifier.y - (scaledSizeY / 2.0f * scale);

        buttonRenderData.destinationRect.w = buttonRenderData.textureSize.x * screenSizeModifier.x * scale;
        buttonRenderData.destinationRect.h = buttonRenderData.textureSize.y * screenSizeModifier.y * scale;

        /*button.addAnimation(titleAnimationTimeMs,
            [&destRect = buttonRenderData.destinationRect,
            animationStartPosition, animationEndPosition
            ](const float delta)
            {
                const float newPositionY{ utils::lerp(
                    animationStartPosition, animationEndPosition, delta) };
                destRect.y = newPositionY;
            }
        );*/

        /*Widget& buttonText{ widgetComponent.addWidget() };

        RenderData& buttonTextRenderData{ buttonText.updateRenderData() };
        buttonTextRenderData = textSystem_->getText(text);
        SDL_SetTextureColorMod(buttonTextRenderData.texture, 0, 0, 0);

        buttonTextRenderData.sourceRect = utils::makeRect(constants::sdlZeroPoint,
            buttonTextRenderData.textureSize);

        auto textScaleDiffX = buttonTextRenderData.textureSize.x * scale - buttonTextRenderData.textureSize.x;
        auto textScaleDiffY = buttonTextRenderData.textureSize.y * scale - buttonTextRenderData.textureSize.y;
        const float textAnimationStartPosition{ screenSize.y + padding };
        const float textAnimationEndPosition{ endPosition * screenSizeModifier.y + textScaleDiffY / 2.0f };
        buttonTextRenderData.destinationRect = utils::makeRect(
            SDL_FPoint
            {
                .x = buttonRenderData.destinationRect.x + buttonTextRenderData.textureSize.x * screenSizeModifier.x + textScaleDiffX * 2.0f,
                .y = textAnimationStartPosition
            },
            screenSizeModifier * buttonTextRenderData.textureSize
        );

        buttonText.addAnimation(titleAnimationTimeMs,
            [&destRect = buttonTextRenderData.destinationRect,
            textAnimationStartPosition, textAnimationEndPosition
            ](const float delta)
            {
                const float newPositionY{ utils::lerp(
                    textAnimationStartPosition, textAnimationEndPosition, delta) };
                destRect.y = newPositionY;
            }
        );*/
    }
}

//void MenuSystem::createMainMenuBackground(const Entity entity, WidgetComponent& widgetComponent) const
//{
//	const SDL_FPoint screenSize{renderSystem_->getScreenSizeF()};
//	for (size_t i{ 0 }; i < mainMenuBackgroundElementsCount; ++i)
//	{
//		createMainMenuBackgroundElement(entity, widgetComponent, screenSize);
//	}
//}

//void MenuSystem::createMainMenuBackgroundElement(const Entity entity, WidgetComponent& widgetComponent, const SDL_FPoint& screenSize) const
//{
//    const auto randomBackgroundElementTextureIndex{
//		randomSystem_->getRandomValue(SDL_Point{.x = 0, .y = backgroundElementTextures.size()})};
//
//	Widget& backgroundElement{ widgetComponent.addWidget() };
//	auto& renderData{backgroundElement.updateRenderData()};
//	const auto& textureInfo{ renderSystem_->createDynamicTexture(
//		backgroundElementTextures[2], entity) };
//	renderData.texture = textureInfo.texture;
//	renderData.textureSize = textureInfo.size;
//	renderData.sourceRect = utils::makeRect(constants::sdlZeroPoint, renderData.textureSize);
//	
//    renderData.destinationRect = createMainMenuBackgroundElementDestRect(screenSize, renderData.textureSize);
//
//    //const SDL_Color randomColor{ randomSystem_->getRandomColor() };
//    //SDL_SetTextureColorMod(renderData.texture, randomColor.r, randomColor.g, randomColor.b);
//	addMainMenuBackgroundElementAnimation(backgroundElement);
//}

//SDL_FRect MenuSystem::createMainMenuBackgroundElementDestRect(const SDL_FPoint& screenSize, const SDL_Point& textureSize) const
//{
//	const float elementRandomScale{ randomSystem_->getRandomValue(mainMenuBackgroundElementsMinMaxScale)};
//    return utils::makeRect(
//        SDL_FPoint{
//        .x = randomSystem_->getRandomValue(SDL_FPoint{.x = 0, .y = screenSize.x}),
//        .y = randomSystem_->getRandomValue(SDL_FPoint{.x = 0, .y = screenSize.y})
//        },
//        SDL_FPoint{
//        .x = textureSize.x * elementRandomScale,
//        .y = textureSize.y * elementRandomScale
//        });
//}
//
//void MenuSystem::addMainMenuBackgroundElementAnimation(Widget& widget) const
//{
//    const auto randomAnimationTime{ randomSystem_->getRandomValue(mainMenuBackgroundElementsMinMaxAnimationTimeMs) };
//    widget.addAnimation(randomAnimationTime,
//        [texture = widget.updateRenderData().texture](const float delta)
//        {
//            utils::lerpOpacity(texture, delta);
//        });
//}

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
