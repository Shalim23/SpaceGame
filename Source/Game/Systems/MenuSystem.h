#pragma once
#include <array>
#include "../Generated/TextureType.h"
#include "../Types/Entity.h"

class World;
class SystemsManager;
class WidgetComponent;
class RenderSystem;
class RandomSystem;
class TextSystem;
class Widget;
struct SDL_FPoint;
struct SDL_Point;
struct SDL_FRect;
enum class GameStateType;
enum class TextType;

class MenuSystem
{

public:
	void preInit(World& world, SystemsManager& systemsManager) {}
	void init(World& world, SystemsManager& systemsManager);
	void postInit(World& world, SystemsManager& systemsManager) {}
	void update(World& world);
	void shutdown() {}

private:
	void createMenuWidget(World& world, const GameStateType gameState) const;

	void createMainMenu(World& world) const;
	void createTitle(WidgetComponent& widgetComponent) const; 
	void createButton(WidgetComponent& widgetComponent,
		const float endPosition, const float padding, const TextType text) const; 
	
	/*void createMainMenuBackground(const Entity entity, WidgetComponent& widgetComponent) const;
	void createMainMenuBackgroundElement(const Entity entity, WidgetComponent& widgetComponent, const SDL_FPoint& screenSize) const;
	SDL_FRect createMainMenuBackgroundElementDestRect(const SDL_FPoint& screenSize, const SDL_Point& textureSize) const;
	void addMainMenuBackgroundElementAnimation(Widget& widget) const;*/
	
	void createInGameMenu(World& world) const;

	WidgetComponent* getMenuWidget(World& world) const;

private:
	RenderSystem* renderSystem_{ nullptr };
	RandomSystem* randomSystem_{ nullptr };
	TextSystem* textSystem_{ nullptr };
	
};