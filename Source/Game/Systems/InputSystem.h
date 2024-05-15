#pragma once
#include "SDL.h"

class World;
class SystemsManager;
struct TransformComponent;

class InputSystem
{
public:
	void preInit(World& world, SystemsManager& systemsManager) {}
	void init(World& world, SystemsManager& systemsManager) {}
	void postInit(World& world, SystemsManager& systemsManager) {}
	void update(World& world);
	void shutdown() {}

private:
	void processRotation(const Uint8* const keyboardState, TransformComponent& transform);
	SDL_FPoint calculateForwardVector(const double rotation) const;

};
