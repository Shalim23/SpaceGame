#pragma once

class World;
class SystemsManager;

class GameplaySystem
{
public:
	void init(World& world, SystemsManager& systemsManager);
	void update(World& world, const double deltaTime) {}
	void shutdown() {}

private:
	void createPlayer(World& world, SystemsManager& systemsManager) const;
};
