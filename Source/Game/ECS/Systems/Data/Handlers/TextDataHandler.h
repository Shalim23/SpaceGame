#pragma once
#include "SDL_ttf.h"
#include "../../../../Generated/TextType.h"
#include "../TextureInfo.h"
#include "../DataDescriptor.h"
#include <vector>
#include <string_view>

class RenderSystem;

class TextDataHandler
{
public:
    void init(RenderSystem* renderSystem, const std::vector<DataDescriptor>& dataDescriptors);
    void shutdown();

    const TextureInfo& getText(const TextType type) const;
    TextureInfo createDynamicText(std::string_view text);

private:
    TTF_Font* loadFontFromRawData(const std::vector<char>& rawData, const int fontSize) const;
    std::vector<TextDescriptor> getTextDescriptors(const DataDescriptor& desc) const;
    TextureInfo createText(std::string_view text) const;
    std::vector<char> getFontRawData(const DataDescriptor& desc) const;

private:
    TTF_Font* font_{ nullptr };
    std::vector<std::pair<TextType, TextureInfo>> staticTexts_;

    RenderSystem* renderSystem_{ nullptr };
};
