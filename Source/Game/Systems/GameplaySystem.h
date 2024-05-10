#pragma once

class World;
class SystemsManager;

class GameplaySystem
{
public:
	void preInit(World& w, SystemsManager& sm){}
	void init(World& w, SystemsManager& sm);
	void update(World& w) {}
	void shutdown() {}

private:
	void createPlayer(World& w, SystemsManager& sm) const;
};
