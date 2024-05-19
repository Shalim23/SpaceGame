#pragma once
#include "../Types/DataDescriptor.h"
#include "../Types/TextureInfo.h"
#include "../Types/Entity.h"
#include "../Generated/TextType.h"
#include "SDL_ttf.h"
#include <vector>

class World;
class SystemsManager;
class RenderSystem;

class DatabaseSystem
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
    void initStaticText();
    std::vector<char> getFontRawData() const;
    std::vector<char> getTextureData(const TextureType type) const;
    std::vector<TextDescriptor> getTextDescriptors() const;
    TTF_Font* loadFontFromRawData(const std::vector<char>& rawData, const int fontSize) const;
    TextureInfo createText(std::string_view text) const;
    TextureInfo createTexture(const TextureType type) const;

private:
    TTF_Font* font_{ nullptr };
    std::vector<DataDescriptor> textureDescriptors_;
    std::vector<std::pair<TextureType, TextureInfo>> textures_;
    std::vector<std::pair<TextType, TextureInfo>> staticTexts_;
    std::vector<std::pair<Entity, SDL_Texture*>> dynamicTextures_;

    RenderSystem* renderSystem_{nullptr};
};