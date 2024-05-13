#include "MenuSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include "../FunctionsLibrary.h"

void MenuSystem::update(World& world)
{
	switch (fl::getCurrentGameState(world))
	{
	case GameStateType::MAIN_MENU:
	{
		if (WidgetComponent* component = getMenuWidget(world))
		{
			//#TODO
			// refresh animations
			// track mouse click position
		}
		else
		{
			createMainMenuWidget(world);
		}
		
		break;
    }
	case GameStateType::INGAME_MENU:
    {
        break;
    }
	}
}

void MenuSystem::createMainMenuWidget(World& world) const
{

}

WidgetComponent* MenuSystem::getMenuWidget(World& world) const
{
	WidgetComponent* comp{nullptr};
	world.forEach<WidgetComponent>([&comp](const Entity entity, WidgetComponent& widgetComponent)
		{
			comp = &widgetComponent;
		});

	return comp;
}
