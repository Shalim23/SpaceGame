#pragma once
#include "SDL.h"
#include "../Types/Entity.h"

class World;
class SystemsManager;
class DataSystem;
struct TransformComponent;

class WorldBoundsSystem
{
public:
	void init(World& world, SystemsManager& systemsManager);
	void update(World& world, const double deltaTime);
	void shutdown() {}

private:
	void generateBackground(World& world);
	bool isPlayerInRange(const TransformComponent& playerTransform) const;

	void createOutOfBoundsWidget(World& world) const;
	Entity createOutOfBoundsEntity(World& world) const;
	void createBackground(World& world) const;
	void createText(World& world) const;

private:
	DataSystem* dbSystem_{nullptr};
	SDL_FPoint screenSizeF_{};
};