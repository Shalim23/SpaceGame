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
	void preInit(World&, SystemsManager&) {}
	void init(World&, SystemsManager&);
	void update(World&);
	void shutdown() {}

private:
	bool isPlayerInRange(const TransformComponent& playerTransform) const;
	void createOutOfBoundsEntity(World& w, const TransformComponent& player_transform);
	std::optional<Entity> getOutOfWorldBoundsComponentEntity(World& w) const;

private:
	const float boundsPixelSize_{3500.0f};
	const int gridSize_{3};

	//#TODO move to widget
	const Uint64 m_fade_in_time_ms{7000};
	const float m_fade_in_timef_ms{static_cast<float>(m_fade_in_time_ms)};
	Uint64 m_fade_in_start{};

	RenderSystem* renderSystem_{nullptr};
};