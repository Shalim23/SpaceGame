#pragma once
#include <array>
#include "../../Generated/TextureType.h"
#include "../Types/Entity.h"

class World;
class SystemsManager;
class WidgetComponent;
class RenderSystem;
class RandomSystem;
class DataSystem;
enum class GameStateType;
enum class TextType : uint32_t;

class MenuSystem
{

public:
	void preInit(World& world, SystemsManager& systemsManager) {}
	void init(World& world, SystemsManager& systemsManager);
	void postInit(World& world, SystemsManager& systemsManager) {}
	void update(World& world, const double deltaTime);
	void shutdown() {}

private:
	void createMenuWidget(World& world, const GameStateType gameState) const;

	void createMainMenu(World& world) const;
	void createTitle(WidgetComponent& widgetComponent) const; 
	void createButton(WidgetComponent& widgetComponent,
		const float endPosition, const float padding, const TextType text) const; 
	
	void createInGameMenu(World& world) const;

	WidgetComponent* getMenuWidget(World& world) const;

private:
	RenderSystem* renderSystem_{ nullptr };
	RandomSystem* randomSystem_{ nullptr };
	DataSystem* dbSystem_{ nullptr };
	
};