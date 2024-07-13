#pragma once
#include "DataDescriptor.h"
#include "TextureInfo.h"
#include "../../Types/Entity.h"
#include "Handlers/TextDataHandler.h"
#include "Handlers/TextureDataHandler.h"
#include <vector>

class World;
class SystemsManager;
class RenderSystem;

class DataSystem
{
public:
	void init(World& world, SystemsManager& systemsManager);
	void update(World& world, const double deltaTime);
	void shutdown();

    const TextureInfo& getTexture(const TextureType type);
    const TextureInfo& getText(const TextType type) const;
    TextureInfo createDynamicText(const Entity entity, std::string_view text);
    TextureInfo createDynamicTexture(const TextureType type, const Entity entity);
    SDL_Surface* createSurface(const TextureType type) const;

private:
	void initDataDescriptors();

#ifndef NDEBUG
    void debugUI();
#endif

private:
    std::vector<DataDescriptor> dataDescriptors_;
    std::tuple<TextDataHandler, TextureDataHandler> dataHandlers_;

#ifndef NDEBUG
    bool showDebugUI_{false};
#endif
};