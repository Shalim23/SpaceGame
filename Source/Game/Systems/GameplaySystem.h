#pragma once

class World;
class SystemsManager;

class GameplaySystem
{
public:
	void preinit(World& w, SystemsManager& sm){}
	void init(World& w, SystemsManager& sm);
	void update(World& w) {}
	void shutdown() {}

private:
	void generateBackground(World& w, SystemsManager& sm);
};
