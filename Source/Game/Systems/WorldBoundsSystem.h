#pragma once

class World;
class SystemsManager;

class WorldBoundsSystem
{
public:
	void preinit(World& w, SystemsManager& sm) {}
	void init(World& w, SystemsManager& sm);
	void update(World& w);
	void shutdown() {}

private:
	bool inRange(const float value, const float min_value, const float max_value) const;

private:
	const float m_bounds_pixel_size{2000.0f};
	const int m_grid_size{15};
};