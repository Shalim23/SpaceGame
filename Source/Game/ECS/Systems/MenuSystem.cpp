#include "MenuSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include "../../GameplayStatics.h"

namespace
{
	constexpr Uint64 titleAnimationTimeMs{1000};
}

void MenuSystem::init(World& world, SystemsManager& systemsManager)
{
	renderSystem_ = &systemsManager.getSystem<RenderSystem>();
	randomSystem_ = &systemsManager.getSystem<RandomSystem>();
    dbSystem_ = &systemsManager.getSystem<DataSystem>();
}

void MenuSystem::update(World& world, const double deltaTime)
{
	const GameStateType gameState{gameplayStatics::getCurrentGameState(world)};
	switch (gameState)
	{
	case GameStateType::MAIN_MENU:
	case GameStateType::INGAME_MENU:
	{
		//#TODO refactor fix
        //if (WidgetComponent* component = getMenuWidget(world))
		//{
		//	for (auto& widget : component->updateWidgets())
		//	{
		//		if (widget.hasAnimation() && widget.isAnimationFinished())
		//		{
		//			
		//		}
		//	}
		//	//#TODO
		//	// track mouse click position
		//}
		//else
		//{
		//	createMenuWidget(world, gameState);
		//}
		
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
    //#TODO refactor fix
 //   const Entity entity{ world.createEntity() };
 //   auto& widget{ world.addComponent<WidgetComponent>(entity) };
 //   widget.setLayer(WidgetLayer::MENU);

	//createTitle(widget);

 //   createButton(widget, 700.0f, 0.0f, TextType::Play);
 //   //createButton(widget, 850.0f, 150.0f, TextType::Exit);

	////createMainMenuBackground(entity, widget);
}

void MenuSystem::createTitle(WidgetComponent& widgetComponent) const
{
	constexpr float scale{4.0f};

    {
        //#TODO refactor fix
        /*Widget& spaceText{ widgetComponent.addWidget() };

        RenderData& spaceTextRenderData{ spaceText.updateRenderData() };
        const auto& textureInfo{dbSystem_->getText(TextType::SPACE)};
        spaceTextRenderData.texture = textureInfo.texture;
        spaceTextRenderData.textureSize = textureInfo.size;

        spaceTextRenderData.sourceRect = utils::makeRect(constants::sdlZeroPoint,
            spaceTextRenderData.textureSize);*/

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

        //#TODO refactor fix
        /*spaceTextRenderData.destinationRect.x = 750.0f;
        spaceTextRenderData.destinationRect.y = 350.0f;
        spaceTextRenderData.scale = scale;*/

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
        //#TODO refactor fix
        /*Widget& gameText{ widgetComponent.addWidget() };

        RenderData& gameTextRenderData{ gameText.updateRenderData() };
        const auto& textureInfo{ dbSystem_->getText(TextType::GAME) };
        gameTextRenderData.texture = textureInfo.texture;
        gameTextRenderData.textureSize = textureInfo.size;

        gameTextRenderData.sourceRect = utils::makeRect(constants::sdlZeroPoint,
            gameTextRenderData.textureSize);*/

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

        //#TODO refactor fix
        /*gameTextRenderData.destinationRect.x = 1000.0f;
        gameTextRenderData.destinationRect.y = 500.0f;
        gameTextRenderData.scale = scale;*/

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
    constexpr float scale{2.0f};

    {
        //#TODO refactor fix
        
        /*Widget& button{ widgetComponent.addWidget() };

        RenderData& buttonRenderData{ button.updateRenderData() };
        const auto& textureInfo{ dbSystem_->getTexture(TextureType::UI_buttonBlue)};
        buttonRenderData.texture = textureInfo.texture;
        buttonRenderData.textureSize = textureInfo.size;

        buttonRenderData.sourceRect = utils::makeRect(constants::sdlZeroPoint,
            buttonRenderData.textureSize);*/

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

        //#TODO refactor fix
        /*buttonRenderData.destinationRect.x = 960.0f;
        buttonRenderData.destinationRect.y = 700.0f;
        buttonRenderData.scale = scale;*/


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




void MenuSystem::createInGameMenu(World& world) const
{
}

WidgetComponent* MenuSystem::getMenuWidget(World& world) const
{
    //#TODO refactor fix
    /*auto& widgets{ world.getComponents<WidgetComponent>() };
    auto iter{std::ranges::find_if(widgets, [](const auto& component)
        { return component.instance.getLayer() == WidgetLayer::MENU; })};

    return iter == widgets.end() ? nullptr : &iter->instance;*/
    return nullptr;
}
