#pragma once
#include <vector>
#include <optional>
#include "../Components/RenderComponent.h"
#include "../Types/Entity.h"

class World;
class SystemsManager;
class RenderSystem;

class ViewportSystem
{
public:
	void init(World& world, SystemsManager& systemsManager);
	void update(World& world, const double deltaTime);
	void shutdown() {}

	const std::vector<std::vector<RenderData>>& getRenderData() const;

private:
	std::optional<RenderData> setupGameObjectRenderData(World& world,
		const Entity entity, const RenderData& sourceData);
	RenderData setupUIRenderData(const RenderData& sourceData);
    SDL_FRect createDestinationRect(const float x, const float y,
        const float w, const float h);

private:
	RenderSystem* renderSystem_{nullptr};
    SDL_FPoint screenSize_{};
    SDL_FPoint halfScreenSize_{};
    SDL_FPoint screenSizeModifier_{};
	std::vector<std::vector<RenderData>> renderDataByLayer_{};
};