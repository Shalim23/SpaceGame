#include "MenuSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include "../GameplayStatics.h"

void MenuSystem::update(World& world)
{
	//switch (gameplayStatics::getCurrentGameState(world))
	//{
	//case GameStateType::MAIN_MENU:
	//{
	//	if (WidgetComponent* component = getMenuWidget(world))
	//	{
	//		//#TODO
	//		// refresh animations
	//		// track mouse click position
	//	}
	//	else
	//	{
	//		createMainMenuWidget(world);
	//	}
	//	
	//	break;
 //   }
	//case GameStateType::INGAME_MENU:
 //   {
 //       break;
 //   }
	//}
}

void MenuSystem::createMainMenuWidget(World& world) const
{

}

WidgetComponent* MenuSystem::getMenuWidget(World& world) const
{
    auto& widgets{ world.getComponents<ComponentType::Widget>() };
	auto iter{std::ranges::find_if(widgets, [](const auto& component)
		{ return component.instance.getLayer() == WidgetLayer::MENU; })};

	return iter == widgets.end() ? nullptr : &iter->instance;
}
