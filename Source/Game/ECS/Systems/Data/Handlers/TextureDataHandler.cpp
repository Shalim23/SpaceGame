#include "TextureDataHandler.h"
#include "../../../World.h"
#include "../FileHandler.h"
#include "../../../SystemsManager.h"
#include "../../RenderSystem.h"
#include "../../../../Generated/DataType.h"

void TextureDataHandler::init(SystemsManager& systemsManager, const std::vector<DataDescriptor>& dataDescriptors)
{
    renderSystem_ = &systemsManager.getSystem<RenderSystem>();

    const auto& texturesDesc{ *std::ranges::find_if(dataDescriptors, [](const DataDescriptor& desc)
        { return desc.id == static_cast<uint32_t>(DataType::Texture); }) };
    dataFilePosition = texturesDesc.position;

    FileHandler dataFile{ utils::getDataFilePath() };

    dataFile.setPosition(dataFilePosition);
    uint32_t texturesCount{};
    if (dataFile.read(reinterpret_cast<char*>(&texturesCount), sizeof(uint32_t)))
    {
        textureDescriptors_.reserve(texturesCount);
        for (size_t i{ 0 }; i < texturesCount; ++i)
        {
            DataDescriptor desc{};
            if (dataFile.read(reinterpret_cast<char*>(&desc), sizeof(desc)))
            {
                textureDescriptors_.push_back(desc);
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

    textures_.reserve(textureDescriptors_.size());
}

void TextureDataHandler::shutdown()
{
    for (const auto& [entity, texture] : dynamicTextures_)
    {
        SDL_DestroyTexture(texture);
    }

    for (const auto& [type, info] : textures_)
    {
        SDL_DestroyTexture(info.texture);
    }

    dynamicTextures_.clear();
    textures_.clear();
}

void TextureDataHandler::addDynamicTexture(const Entity entity, SDL_Texture* texture)
{
    dynamicTextures_.emplace_back(std::make_pair(entity, texture));
}

std::vector<char> TextureDataHandler::getTextureData(const TextureType type) const
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

    FileHandler dataFile{ utils::getDataFilePath() };
    const size_t positionOffset{sizeof(uint32_t) + sizeof(DataDescriptor) * textureDescriptors_.size()};
    dataFile.setPosition(dataFilePosition + positionOffset + iter->position);


    std::vector<char> buffer(iter->size);
    if (dataFile.read(buffer.data(), iter->size))
    {
        return buffer;
    }

    utils::showMessageBox(__FUNCTION__, "data.bin is corrupted!");
    throw std::exception{};
}

TextureInfo TextureDataHandler::createTexture(const TextureType type) const
{
    const auto& textureRawData{ getTextureData(type) };
    SDL_Texture* texture{ renderSystem_->createTextureFromData(textureRawData) };
    const SDL_Point textureSize{ renderSystem_->getTextureSize(texture) };

    return TextureInfo{ .texture = texture, .size = textureSize };
}

void TextureDataHandler::clearDynamicTextures(World& world)
{
    const auto& renderComponents{ world.getComponents<RenderComponent>() };
    decltype(dynamicTextures_) dynamicTexturesToRemove;

    for (const auto& [entity, texture] : dynamicTextures_)
    {
        if (!utils::containsIf(renderComponents, [&entity](const auto& component)
            {   return entity == component.entity; }))
        {
            dynamicTexturesToRemove.push_back({ entity, texture });
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

const TextureInfo& TextureDataHandler::getTexture(const TextureType type)
{
    const auto iter{ std::ranges::find_if(textures_,
                [type](const auto& texture) { return texture.first == type; }) };
    if (iter != textures_.end())
    {
        return iter->second;
    }

    return textures_.emplace_back(std::make_pair(type, createTexture(type))).second;
}

TextureInfo TextureDataHandler::createDynamicTexture(const TextureType type, const Entity entity)
{
    TextureInfo info{ createTexture(type) };
    addDynamicTexture(entity, info.texture);
    return info;
}

SDL_Surface* TextureDataHandler::createSurface(const TextureType type) const
{
    const auto& textureRawData{ getTextureData(type) };
    return renderSystem_->createSurface(textureRawData);
}
