#pragma once

class World;
class SystemsManager;

class {{ name }}
{
public:
	void preInit(World&, SystemsManager&) {}
	void init(World&, SystemsManager&) {}
	void update(World&) {}
	void shutdown() {}
};
