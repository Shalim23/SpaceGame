#pragma once

class World;
class SystemsManager;
class WidgetComponent;

class MenuSystem
{

public:
	void preInit(World& world, SystemsManager& systemsManager) {}
	void init(World& world, SystemsManager& systemsManager) {}
	void postInit(World& world, SystemsManager& systemsManager) {}
	void update(World& world);
	void shutdown() {}

private:
	void createMainMenuWidget(World& world) const;
	WidgetComponent* getMenuWidget(World& world) const;
};