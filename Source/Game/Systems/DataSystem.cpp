#include "DataSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include "../Types/Exceptions.h"
#include "../Types/FileHandler.h"
#include <fstream>

void DataSystem::update(World& world, const double deltaTime)
{
    const auto& renderComponents{ world.getComponents<RenderComponent>() };
    decltype(dynamicTextures_) dynamicTexturesToRemove;

    for (const auto& [entity, texture] : dynamicTextures_)
    {
        if (!utils::containsIf(renderComponents, [&entity](const auto& component)
            {   return entity == component.entity; }))
        {
            dynamicTexturesToRemove.push_back({entity, texture});
        }
    }

    for (const auto& [entity, texture] : dynamicTexturesToRemove)
    {
        SDL_DestroyTexture(texture);

        std::erase_if(dynamicTextures_, [entity](const auto& data)
            {
                return entity == data.first;
            });
    }
}

void DataSystem::shutdown()
{
    for (const auto& [type, info] : staticTexts_)
    {
        SDL_DestroyTexture(info.texture);
    }

    for (const auto& [entity, texture] : dynamicTextures_)
    {
        SDL_DestroyTexture(texture);
    }

    for (const auto& [type, info] : textures_)
    {
        SDL_DestroyTexture(info.texture);
    }

    staticTexts_.clear();
    dynamicTextures_.clear();
    textures_.clear();
    
    TTF_CloseFont(font_);
}

void DataSystem::load(SystemsManager& systemsManager)
{
    renderSystem_ = &systemsManager.getSystem<RenderSystem>();
    
    initDataDescriptors();

    initTexturesDescriptors();

    //const std::vector<char> fontRawData{ getFontRawData() };
    //font_ = loadFontFromRawData(fontRawData, 40);

    //initStaticText();
}

const TextureInfo& DataSystem::getTexture(const TextureType type)
{
    const auto iter{ std::ranges::find_if(textures_,
            [type](const auto& texture) { return texture.first == type; }) };
    if (iter != textures_.end())
    {
        return iter->second;
    }

    return textures_.emplace_back(std::make_pair(type, createTexture(type))).second;
}

const TextureInfo& DataSystem::getText(const TextType type) const
{
    auto iter{ std::ranges::find_if(staticTexts_,
            [type](const auto& text) { return text.first == type; })};
    return iter->second;
}

TextureInfo DataSystem::createDynamicText(const Entity entity, std::string_view text)
{
    TextureInfo data{ createText(text) };
    addDynamicTexture(entity, data.texture);
    return data;
}

TextureInfo DataSystem::createDynamicTexture(const TextureType type, const Entity entity)
{
    TextureInfo info{ createTexture(type) };
    addDynamicTexture(entity, info.texture);
    return info;
}

SDL_Surface* DataSystem::createSurface(const TextureType type) const
{
    const auto& textureRawData{ getTextureData(type) };
    return renderSystem_->createSurface(textureRawData);
}

void DataSystem::addDynamicTexture(const Entity entity, SDL_Texture* texture)
{
    dynamicTextures_.emplace_back(std::make_pair(entity, texture));
}

void DataSystem::initTexturesDescriptors()
{
    std::ifstream descriptorsFile("Data/texturesDescriptors.bin", std::ios::binary);
    if (!descriptorsFile.is_open())
    {
        utils::showMessageBox(__FUNCTION__, "texturesDescriptors.bin is missing!");
        throw std::exception{};
    }

    DataDescriptor desc{};
    while (descriptorsFile.read(reinterpret_cast<char*>(&desc), sizeof(DataDescriptor)))
    {
        textureDescriptors_.emplace_back(desc);
    }

    descriptorsFile.close();
    textures_.reserve(textureDescriptors_.size());
}

void DataSystem::initDataDescriptors()
{
    FileHandler dataFile{ "Data/data.bin" };

    uint32_t dataTypesCount{};
    if (dataFile.read(reinterpret_cast<char*>(&dataTypesCount), sizeof(uint32_t)))
    {
        for (size_t i{ 0 }; i < dataTypesCount; ++i)
        {
            DataDescriptor desc{};
            if (dataFile.read(reinterpret_cast<char*>(&desc), sizeof(DataDescriptor)))
            {
                dataDescriptors_.emplace_back(desc);
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
}

void DataSystem::initStaticText()
{
    const std::vector<TextDescriptor> textDescriptors{ getTextDescriptors() };

    staticTexts_.reserve(textDescriptors.size());

    for (const auto& desc : textDescriptors)
    {
        staticTexts_.emplace_back(std::make_pair(
            static_cast<TextType>(desc.id), createText(desc.text)));
    }
}

std::vector<char> DataSystem::getFontRawData() const
{
    std::ifstream fontFile("Data/gameFont.bin", std::ios::binary);
    if (!fontFile.is_open())
    {
        utils::showMessageBox(__FUNCTION__, "gameFont.bin is missing!");
        throw SystemInitException{};
    }

    std::vector<char> fontRawData(std::istreambuf_iterator<char>(fontFile), {});

    fontFile.close();

    return fontRawData;
}

std::vector<char> DataSystem::getTextureData(const TextureType type) const
{
    const auto textureTypeTnt{ static_cast<uint32_t>(type) };
    const auto iter{ std::ranges::find_if(textureDescriptors_,
        [textureTypeTnt](const DataDescriptor& desc) { return desc.id == textureTypeTnt; }) };
    if (iter == textureDescriptors_.end())
    {
        std::stringstream ss;
        ss << "Unknown TextureType " << textureTypeTnt << "!";
        utils::showMessageBox(__FUNCTION__, ss.str().c_str());
        throw std::exception{};
    }

    std::ifstream texturesFile("Data/textures.bin", std::ios::binary);
    if (!texturesFile.is_open())
    {
        utils::showMessageBox(__FUNCTION__, "textures.bin is missing!");
        throw std::exception{};
    }

    texturesFile.seekg(iter->position, std::ios::beg);
    if (!texturesFile)
    {
        utils::showMessageBox(__FUNCTION__, "textures.bin data is corrupted!");
        throw std::exception{};
    }

    std::vector<char> buffer(iter->size);
    texturesFile.read(buffer.data(), iter->size);
    if (!texturesFile)
    {
        utils::showMessageBox(__FUNCTION__, "textures.bin data is corrupted!");
        throw std::exception{};
    }

    texturesFile.close();
    return buffer;
}

std::vector<TextDescriptor> DataSystem::getTextDescriptors() const
{
    std::ifstream textDescriptorsFile("Data/textDescriptors.bin", std::ios::binary);
    if (!textDescriptorsFile.is_open())
    {
        utils::showMessageBox(__FUNCTION__, "textDescriptors.bin is missing!");
        throw SystemInitException{};
    }

    std::vector<TextDescriptor> textDescriptors;

    while (!textDescriptorsFile.eof())
    {
        TextDescriptor desc{};
        uint32_t len{};
        if (textDescriptorsFile.read(reinterpret_cast<char*>(&desc.id), sizeof(desc.id)) &&
            textDescriptorsFile.read(reinterpret_cast<char*>(&len), sizeof(len)))
        {
            std::vector<char> buffer(len);
            if (textDescriptorsFile.read(buffer.data(), len))
            {
                desc.text.assign(buffer.begin(), buffer.end());
                textDescriptors.push_back(desc);
            }
        }
    }

    textDescriptorsFile.close();

    return textDescriptors;
}

TTF_Font* DataSystem::loadFontFromRawData(const std::vector<char>& rawData, const int fontSize) const
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

TextureInfo DataSystem::createText(std::string_view text) const
{
    SDL_Surface* textSurface{ TTF_RenderText_Solid(font_, text.data(),
            SDL_Color{.r = 255, .g = 255, .b = 255, .a = 255}) };
    if (!textSurface)
    {
        auto a = SDL_GetError();
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

    return TextureInfo{.texture = texture,
        .size = textureSize };
}

TextureInfo DataSystem::createTexture(const TextureType type) const
{
    const auto& textureRawData{ getTextureData(type) };
    SDL_Texture* texture{ renderSystem_->createTextureFromData(textureRawData) };
    const SDL_Point textureSize{ renderSystem_->getTextureSize(texture) };

    return TextureInfo{ .texture = texture, .size = textureSize };
}
