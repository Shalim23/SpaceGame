#pragma once

class World;
class SystemsManager;

class {{ name }}
{
public:
	void preInit(World& w, SystemsManager& sm) {}
	void init(World& w, SystemsManager& sm) {}
	void update(World& w) {}
	void shutdown() {}
};
