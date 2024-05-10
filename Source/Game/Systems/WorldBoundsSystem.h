#pragma once
#include "SDL.h"
#include "../Types/Entity.h"
#include <optional>

class World;
class SystemsManager;
class RenderSystem;
struct TransformComponent;

class WorldBoundsSystem
{
public:
	void init(World& world, SystemsManager& systemsManager){}
	void postInit(World& world, SystemsManager& systemsManager);
	void update(World& world);
	void shutdown() {}

private:
	bool isPlayerInRange(const TransformComponent& playerTransform) const;
	void createOutOfBoundsEntity(World& world) const;
	std::optional<Entity> getOutOfWorldBoundsComponentEntity(World& world) const;

private:
	const float boundsPixelSize_{3500.0f};
	const int gridSize_{3};

	RenderSystem* renderSystem_{nullptr};
};