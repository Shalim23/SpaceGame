#pragma once
#include <vector>
#include "../DataDescriptor.h"
#include "../TextureInfo.h"
#include "../../../Types/Entity.h"
#include "SDL.h"

class World;
class SystemsManager;
class RenderSystem;

class TextureDataHandler
{
public:
    void init(SystemsManager& systemsManager, const std::vector<DataDescriptor>& dataDescriptors);
    void shutdown();
    void clearDynamicTextures(World& world);
    const TextureInfo& getTexture(const TextureType type);
    TextureInfo createDynamicTexture(const TextureType type, const Entity entity);
    SDL_Surface* createSurface(const TextureType type) const;
    void addDynamicTexture(const Entity entity, SDL_Texture* texture);

private:
    std::vector<char> getTextureData(const TextureType type) const;
    TextureInfo createTexture(const TextureType type) const;

private:
    std::vector<DataDescriptor> textureDescriptors_;
    std::vector<std::pair<TextureType, TextureInfo>> textures_;
    std::vector<std::pair<Entity, SDL_Texture*>> dynamicTextures_;
    uint32_t dataFilePosition{};
    RenderSystem* renderSystem_{ nullptr };
};
