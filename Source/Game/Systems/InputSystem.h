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
	SDL_FPoint calculateForwardVector(const double rotation) const;

private:
	const double RotationPerSecond{20.0};
	const float MovementPerSecond{50.0};
};
