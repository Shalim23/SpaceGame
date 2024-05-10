#pragma once

class World;
class SystemsManager;

class {{ name }}
{
public:
	void init(World& world, SystemsManager& systemsManager) {}
	void postInit(World& world, SystemsManager& systemsManager) {}
	void update(World& world) {}
	void shutdown() {}
};
