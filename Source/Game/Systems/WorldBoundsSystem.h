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
	void createOutOfBoundsEntity(World& world, const TransformComponent& playerTransform);
	std::optional<Entity> getOutOfWorldBoundsComponentEntity(World& world) const;

private:
	const float boundsPixelSize_{3500.0f};
	const int gridSize_{3};

	//#TODO move to widget
	const Uint64 m_fade_in_time_ms{7000};
	const float m_fade_in_timef_ms{static_cast<float>(m_fade_in_time_ms)};
	Uint64 m_fade_in_start{};

	RenderSystem* renderSystem_{nullptr};
};