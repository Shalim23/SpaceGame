#pragma once

class World;
class SystemsManager;

class RenderSystem
{
public:
	void init(World& w, SystemsManager& sm) {}
	void update(World& w) {}
	void shutdown() {}
};
