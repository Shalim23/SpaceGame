#pragma once
#include "SDL.h"

class World;
class SystemsManager;
class RenderSystem;
struct TransformComponent;

class WorldBoundsSystem
{
public:
	void preinit(World& w, SystemsManager& sm) {}
	void init(World& w, SystemsManager& sm);
	void update(World& w);
	void shutdown() {}

private:
	bool isPlayerInRange(const TransformComponent& player_transform) const;
	bool inRange(const float value, const float min_value, const float max_value) const;
	void createOutOfBoundsEntity(World& w, const TransformComponent& player_transform);

private:
	const float m_bounds_pixel_size{3500.0f};
	const int m_grid_size{20};

	const Uint64 m_fade_in_time_ms{7000};
	const float m_fade_in_timef_ms{static_cast<float>(m_fade_in_time_ms)};
	Uint64 m_fade_in_start{};

	RenderSystem* m_render_system{nullptr};
};