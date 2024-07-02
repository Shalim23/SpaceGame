#include "DataSystem.h"
#include "../../World.h"
#include "../../SystemsManager.h"
#include "../../Types/Exceptions.h"
#include "FileHandler.h"
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
    std::apply([](auto&&... handler) {((handler.shutdown()), ...); }, dataHandlers_);
    
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

void DataSystem::load(SystemsManager& systemsManager)
{
    renderSystem_ = &systemsManager.getSystem<RenderSystem>();
    
    initDataDescriptors();

    std::apply([this](auto&&... handler)
        {((handler.init(renderSystem_, dataDescriptors_)), ...); }, dataHandlers_);

    // #TODO
    //initTexturesDescriptors();

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
    return std::get<TextDataHandler>(dataHandlers_).getText(type);
}

TextureInfo DataSystem::createDynamicText(const Entity entity, std::string_view text)
{
    TextureInfo data{ std::get<TextDataHandler>(dataHandlers_).createDynamicText(text) };
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
    FileHandler dataFile{ utils::getDataFilePath() };

    uint32_t dataTypesCount{};
    if (dataFile.read(reinterpret_cast<char*>(&dataTypesCount), sizeof(uint32_t)))
    {
        const size_t dataFileOffset{ sizeof(uint32_t) + (sizeof(DataDescriptor) * dataTypesCount) };
        for (size_t i{ 0 }; i < dataTypesCount; ++i)
        {
            DataDescriptor desc{};
            if (dataFile.read(reinterpret_cast<char*>(&desc), sizeof(DataDescriptor)))
            {
                desc.position += dataFileOffset;
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

TextureInfo DataSystem::createTexture(const TextureType type) const
{
    const auto& textureRawData{ getTextureData(type) };
    SDL_Texture* texture{ renderSystem_->createTextureFromData(textureRawData) };
    const SDL_Point textureSize{ renderSystem_->getTextureSize(texture) };

    return TextureInfo{ .texture = texture, .size = textureSize };
}
