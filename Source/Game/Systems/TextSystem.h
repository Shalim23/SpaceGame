#pragma once
#include <vector>
#include <string_view>
#include "SDL_ttf.h"
#include "../Types/Entity.h"
#include "../Types/RenderData.h"
#include "../Generated/TextType.h"

class World;
class SystemsManager;
class RenderSystem;

class TextSystem
{

struct TextDescriptor
{
    uint32_t id;
    uint32_t len;
    std::string text;
};

struct StaticText
{
    TextType id;
    RenderData data;
};

public:
	void preInit(World& world, SystemsManager& systemsManager) {}
	void init(World& world, SystemsManager& systemsManager);
	void postInit(World& world, SystemsManager& systemsManager){}
	void update(World& world, const double deltaTime){}
	void shutdown();

	RenderData getText(const TextType type) const;
	RenderData createDynamicText(const Entity entity, std::string_view text);
	RenderData createText(std::string_view text);

private:
	std::vector<char> getFontRawData() const;
	std::vector<TextDescriptor> getTextDescriptors() const;
	TTF_Font* loadFontFromRawData(const std::vector<char>& rawData, const int fontSize) const;
	void initStaticText();

private:
	TTF_Font* font_{nullptr};
	RenderSystem* renderSystem_{nullptr};
	std::vector<StaticText> staticTexts_;
};