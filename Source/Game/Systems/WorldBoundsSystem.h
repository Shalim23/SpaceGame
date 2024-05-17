#pragma once
#include "SDL.h"
#include "../Types/Entity.h"
#include <optional>

class World;
class SystemsManager;
class RenderSystem;
class TextSystem;
class WidgetComponent;
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

	void createOutOfBoundsEntity(World& world) const;
	void createBackgroundWidget(WidgetComponent& widgetComponent) const;
	void createTextWidget(const Entity entity, WidgetComponent& widgetComponent) const;

private:
	RenderSystem* renderSystem_{nullptr};
	TextSystem* textSystem_{nullptr};
};