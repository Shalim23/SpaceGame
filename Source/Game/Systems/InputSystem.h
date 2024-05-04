#pragma once
#include "SDL.h"

class World;
class SystemsManager;
struct TransformComponent;

class InputSystem
{
public:
	void preinit(World& w, SystemsManager& sm) {}
	void init(World& w, SystemsManager& sm) {}
	void update(World& w);
	void shutdown() {}

private:
	void processRotation(const Uint8* keyboardState, TransformComponent& t);

private:
	const double RotationPerSecond{20.0};
};
