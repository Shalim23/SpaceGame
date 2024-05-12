#pragma once
#include <vector>
#include <string_view>
#include "SDL_ttf.h"
#include "../Types/Entity.h"

class World;
class SystemsManager;
class RenderSystem;
struct RenderData;

class TextSystem
{
struct DynamicTextsToEntity
{
	Entity entity;
	std::vector<SDL_Texture*> textTextures;
};

public:
	void init(World& world, SystemsManager& systemsManager);
	void postInit(World& world, SystemsManager& systemsManager){}
	void update(World& world);
	void shutdown();

	RenderData createText(const Entity entity, std::string_view text, const SDL_Color& color);

private:
	std::vector<char> getFontRawData() const;
	TTF_Font* loadFontFromRawData(const std::vector<char>& rawData, const int fontSize) const;
	void addDynanicText(const Entity entity, SDL_Texture* texture);

private:
	TTF_Font* font_{nullptr};
	RenderSystem* renderSystem_{nullptr};
	std::vector<DynamicTextsToEntity> dynamicTexts_;
};