#pragma once

class World;
class SystemsManager;

class ResourcesSystem
{
public:
	void init(World& world, SystemsManager& systemsManager) {}
	void update(World& world, const double deltaTime) {}
	void shutdown() {}
	void loadResources();
};