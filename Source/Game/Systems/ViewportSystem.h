#pragma once
#include <vector>
#include "../Types/RenderData.h"

class World;
class SystemsManager;
class RenderSystem;

class ViewportSystem
{
public:
	void init(World& world, SystemsManager& systemsManager);
	void update(World& world, const double deltaTime);
	void shutdown() {}

	const std::vector<RenderData>& getRenderData() const;

private:
	void prepareGameObjectsRenderData(World& world);
	void prepareUIRenderData(World& world);

private:
	RenderSystem* renderSystem_;
	std::vector<RenderData> renderData_;
};