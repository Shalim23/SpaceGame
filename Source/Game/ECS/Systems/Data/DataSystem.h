#pragma once
#include "DataDescriptor.h"
#include "TextureInfo.h"
#include "../../Types/Entity.h"
#include "Handlers/TextDataHandler.h"
#include <vector>

class World;
class SystemsManager;
class RenderSystem;

class DataSystem
{
public:
	void init(World& world, SystemsManager& systemsManager){}
	void update(World& world, const double deltaTime);
	void shutdown();
	void load(SystemsManager& systemsManager);

    const TextureInfo& getTexture(const TextureType type);
    const TextureInfo& getText(const TextType type) const;
    TextureInfo createDynamicText(const Entity entity, std::string_view text);
    TextureInfo createDynamicTexture(const TextureType type, const Entity entity);
    SDL_Surface* createSurface(const TextureType type) const;

private:
    void addDynamicTexture(const Entity entity, SDL_Texture* texture);
	void initTexturesDescriptors();
	void initDataDescriptors();
    std::vector<char> getTextureData(const TextureType type) const;
    TextureInfo createTexture(const TextureType type) const;

private:
    std::vector<DataDescriptor> dataDescriptors_;
    std::vector<DataDescriptor> textureDescriptors_;
    std::vector<std::pair<TextureType, TextureInfo>> textures_;
    std::vector<std::pair<Entity, SDL_Texture*>> dynamicTextures_;

    RenderSystem* renderSystem_{nullptr};

    std::tuple<TextDataHandler> dataHandlers_;
};