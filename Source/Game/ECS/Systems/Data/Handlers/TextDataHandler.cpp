#include "TextDataHandler.h"
#include "../DataDescriptor.h"
#include "../../../../Utils.h"
#include "../../../Types/Exceptions.h"
#include "../../RenderSystem.h"
#include "../../../../Generated/DataType.h"
#include "../FileHandler.h"
#include <filesystem>

void TextDataHandler::init(RenderSystem* renderSystem, const std::vector<DataDescriptor>& dataDescriptors)
{
    renderSystem_ = renderSystem;

    {
        const auto& fontDesc{*std::ranges::find_if(dataDescriptors, [](const DataDescriptor& desc)
        { return desc.id == static_cast<uint32_t>(DataType::Font); })};

        const std::vector<char> fontRawData{ getFontRawData(fontDesc) };
        font_ = loadFontFromRawData(fontRawData, 40);
    }

    const auto& textDesc{ *std::ranges::find_if(dataDescriptors, [](const DataDescriptor& desc)
    { return desc.id == static_cast<uint32_t>(DataType::Text); }) };

    const std::vector<TextDescriptor> textDescriptors{ getTextDescriptors(textDesc) };

    staticTexts_.reserve(textDescriptors.size());

    for (const auto& desc : textDescriptors)
    {
        staticTexts_.emplace_back(std::make_pair(
            static_cast<TextType>(desc.id), createText(desc.text)));
    }
}

void TextDataHandler::shutdown()
{
    for (const auto& [type, info] : staticTexts_)
    {
        SDL_DestroyTexture(info.texture);
    }

    staticTexts_.clear();
    
    TTF_CloseFont(font_);
}

const TextureInfo& TextDataHandler::getText(const TextType type) const
{
    auto iter{ std::ranges::find_if(staticTexts_,
                [type](const auto& text) { return text.first == type; }) };
    return iter->second;
}

TextureInfo TextDataHandler::createDynamicText(std::string_view text)
{
    return createText(text);
}

TTF_Font* TextDataHandler::loadFontFromRawData(const std::vector<char>& rawData, const int fontSize) const
{
    SDL_RWops* rw{ SDL_RWFromConstMem(rawData.data(), rawData.size()) };
    if (!rw)
    {
        utils::showMessageBox(__FUNCTION__, "Failed to create RWops!");
        throw SystemInitException{};
    }

    TTF_Font* font{ TTF_OpenFontRW(rw, 1, fontSize) };
    if (!font)
    {
        utils::showMessageBox(__FUNCTION__, "Failed to load font!");
        throw SystemInitException{};
        SDL_RWclose(rw);
    }

    return font;
}

std::vector<TextDescriptor> TextDataHandler::getTextDescriptors(const DataDescriptor& desc) const
{
    FileHandler dataFile{ utils::getDataFilePath() };

    std::vector<TextDescriptor> textDescriptors;

    dataFile.setPosition(desc.position);
    
    uint32_t textsCount{};
    if (dataFile.read(reinterpret_cast<char*>(&textsCount), sizeof(uint32_t)))
    {
        textDescriptors.reserve(textsCount);
        for (size_t i{ 0 }; i < textsCount; ++i)
        {
            TextDescriptor desc{};
            uint32_t len{};
            if (dataFile.read(reinterpret_cast<char*>(&desc.id), sizeof(desc.id)) &&
                dataFile.read(reinterpret_cast<char*>(&len), sizeof(len)))
            {
                std::vector<char> buffer(len);
                if (dataFile.read(buffer.data(), len))
                {
                    desc.text.assign(buffer.begin(), buffer.end());
                    textDescriptors.push_back(desc);
                }
            }
            else
            {
                utils::showMessageBox(__FUNCTION__, "data.bin is corrupted!");
                throw std::exception{};
            }
        }
    }
    else
    {
        utils::showMessageBox(__FUNCTION__, "data.bin is corrupted!");
        throw std::exception{};
    }

    return textDescriptors;
}

TextureInfo TextDataHandler::createText(std::string_view text) const
{
    SDL_Surface* textSurface{ TTF_RenderText_Solid(font_, text.data(),
                SDL_Color{.r = 255, .g = 255, .b = 255, .a = 255}) };
    if (!textSurface)
    {
        utils::showMessageBox(__FUNCTION__, "Failed to create text surface!");
        throw std::exception{};
    }

    SDL_Texture* texture{ renderSystem_->createTextureFromSurface(textSurface) };
    if (!texture)
    {
        utils::showMessageBox(__FUNCTION__, "Failed to create texture from text surface!");
        throw std::exception{};
    }

    const SDL_Point textureSize{ .x = textSurface->w, .y = textSurface->h };
    SDL_FreeSurface(textSurface);

    return TextureInfo{ .texture = texture,
        .size = textureSize };
}

std::vector<char> TextDataHandler::getFontRawData(const DataDescriptor& desc) const
{
    std::vector<char> fontRawData(desc.size);
    FileHandler dataFile{ utils::getDataFilePath() };
    dataFile.setPosition(desc.position);
    dataFile.read(fontRawData.data(), desc.size);
    return fontRawData;
}
