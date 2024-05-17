#include "TextSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include "../Types/Exceptions.h"
#include <fstream>

void TextSystem::init(World& world, SystemsManager& systemsManager)
{
    renderSystem_ = &systemsManager.getSystem<RenderSystem>();

    const std::vector<char> fontRawData{ getFontRawData() };
    font_ = loadFontFromRawData(fontRawData, 40);

    initStaticText();
}

void TextSystem::shutdown()
{
    for (const auto& staticText : staticTexts_)
    {
        SDL_DestroyTexture(staticText.data.texture);
    }
    
    TTF_CloseFont(font_);
}

RenderData TextSystem::getText(const TextType type) const
{
    auto iter{std::ranges::find_if(staticTexts_,
        [type](const StaticText& text){ return text.id == type; })};
    return iter->data;
}

RenderData TextSystem::createDynamicText(const Entity entity, std::string_view text)
{
    RenderData data{createText(text)};
    renderSystem_->addDynamicTexture(entity, data.texture);
    return data;
}

RenderData TextSystem::createText(std::string_view text)
{
    SDL_Surface* textSurface{ TTF_RenderText_Solid(font_, text.data(),
        SDL_Color{.r = 255, .g = 255, .b = 255, .a = 255})};
    if (!textSurface)
    {
        auto a = SDL_GetError();
        renderSystem_->showMessageBox(__FUNCTION__, "Failed to create text surface!");
        throw std::exception{};
    }

    SDL_Texture* texture{ renderSystem_->createTextureFromSurface(textSurface) };
    if (!texture)
    {
        renderSystem_->showMessageBox(__FUNCTION__, "Failed to create texture from text surface!");
        throw std::exception{};
    }

    RenderData renderData{};
    renderData.texture = texture;
    renderData.textureSize.x = textSurface->w;
    renderData.textureSize.y = textSurface->h;

    SDL_FreeSurface(textSurface);

    return renderData;
}

std::vector<char> TextSystem::getFontRawData() const
{
    std::ifstream fontFile("Data/gameFont.bin", std::ios::binary);
    if (!fontFile.is_open())
    {
        renderSystem_->showMessageBox(__FUNCTION__, "gameFont.bin is missing!");
        throw SystemInitException{};
    }

    std::vector<char> fontRawData(std::istreambuf_iterator<char>(fontFile), {});

    fontFile.close();

    return fontRawData;
}

std::vector<TextSystem::TextDescriptor> TextSystem::getTextDescriptors() const
{
    std::ifstream textDescriptorsFile("Data/textDescriptors.bin", std::ios::binary);
    if (!textDescriptorsFile.is_open())
    {
        renderSystem_->showMessageBox(__FUNCTION__, "textDescriptors.bin is missing!");
        throw SystemInitException{};
    }

    std::vector<TextDescriptor> textDescriptors;

    while (!textDescriptorsFile.eof())
    {
        TextDescriptor desc{};
        if (textDescriptorsFile.read(reinterpret_cast<char*>(&desc.id), sizeof(desc.id)) &&
            textDescriptorsFile.read(reinterpret_cast<char*>(&desc.len), sizeof(desc.len)))
        {
            std::vector<char> buffer(desc.len);
            if (textDescriptorsFile.read(buffer.data(), desc.len))
            {
                desc.text.assign(buffer.begin(), buffer.end());
                textDescriptors.push_back(desc);
            }
        }
    }

    textDescriptorsFile.close();

    return textDescriptors;
}

TTF_Font* TextSystem::loadFontFromRawData(const std::vector<char>& rawData, const int fontSize) const
{
    SDL_RWops* rw{SDL_RWFromConstMem(rawData.data(), rawData.size())};
    if (!rw)
    {
        renderSystem_->showMessageBox(__FUNCTION__, "Failed to create RWops!");
        throw SystemInitException{};
    }

    TTF_Font* font{TTF_OpenFontRW(rw, 1, fontSize)};
    if (!font)
    {
        renderSystem_->showMessageBox(__FUNCTION__, "Failed to load font!");
        throw SystemInitException{};
        SDL_RWclose(rw);
    }

    return font;
}

void TextSystem::initStaticText()
{
    const std::vector<TextDescriptor> textDescriptors{getTextDescriptors()};

    staticTexts_.reserve(textDescriptors.size());

    for (const auto& desc : textDescriptors)
    {
        staticTexts_.emplace_back(StaticText{.id = static_cast<TextType>(desc.id),
            .data = createText(desc.text)});
    }
}
