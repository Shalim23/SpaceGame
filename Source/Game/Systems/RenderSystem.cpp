#include "RenderSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include "../Types/Exceptions.h"
#include "../Utils.h"
#include "../GameplayStatics.h"
#include "../Constants.h"
#include "SDL_image.h"
#include <fstream>
#include <sstream>

void RenderSystem::preInit(World& world, SystemsManager& systemsManager)
{
    constexpr Uint32 fullscreenFlag{ 0 };
    //constexpr Uint32 fullscreenFlag{ SDL_WINDOW_FULLSCREEN_DESKTOP };

    window_ = SDL_CreateWindow("Space Game", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 1920, 1080, fullscreenFlag);
    if (!window_)
    {
        showMessageBox(__FUNCTION__, "Failed to create window!");
        throw SystemInitException{};
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer_)
    {
        showMessageBox(__FUNCTION__, "Failed to create renderer!");
        throw SystemInitException{};
    }

    initTexturesDescriptors();
}

void RenderSystem::update(World& world)
{
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer_);

    gameplayStatics::clearWidgetDynamicTextures(dynamicTextures_, world);
    processSpriteData(world);
    processWidgetData(world);

    SDL_RenderPresent(renderer_);
}

void RenderSystem::shutdown()
{
    for (const auto& textures : dynamicTextures_)
    {
        for (const auto& texture : textures.textures)
        {
            SDL_DestroyTexture(texture);
        }
    }
    
    for (const auto& texture : textures_)
    {
        SDL_DestroyTexture(texture.texture);
    }
    
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
}

void RenderSystem::showMessageBox(const char* title, const char* message) const
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message, window_);
}

const Texture& RenderSystem::getTexture(const TextureType type)
{
    const auto iter{ std::ranges::find_if(textures_,
        [type](const Texture& texture) { return texture.type == type; }) };
    if (iter != textures_.end())
    {
        return *iter;
    }

    return textures_.emplace_back(createTexture(type));
}

Texture RenderSystem::createDynamicTexture(const TextureType type, const Entity entity)
{
    const Texture texture{createTexture(type)};
    addDynamicTexture(entity, texture.texture);
    return texture;
}

void RenderSystem::addDynamicTexture(const Entity entity, SDL_Texture* texture)
{
    auto iter{ std::ranges::find_if(dynamicTextures_, [entity](const DynamicTexture& texts)
            { return texts.entity == entity; }) };
    DynamicTexture* texts{ iter == dynamicTextures_.end() ?
        &dynamicTextures_.emplace_back(DynamicTexture{.entity = entity }) :
        &(*iter) };

    texts->textures.push_back(texture);
}

SDL_Texture* RenderSystem::createTextureFromData(const std::vector<char>& rawData) const
{
    return IMG_LoadTexture_RW(renderer_, SDL_RWFromConstMem(rawData.data(), rawData.size()), 1);
}

SDL_Texture* RenderSystem::createTextureFromSurface(SDL_Surface* surface) const
{
    return SDL_CreateTextureFromSurface(renderer_, surface);
}

SDL_Surface* RenderSystem::createSurface(const TextureType type) const
{
    const auto& textureRawData{ getTextureData(type) };
    return IMG_Load_RW(SDL_RWFromConstMem(textureRawData.data(), textureRawData.size()), 1);
}

SDL_Point RenderSystem::getTextureSize(SDL_Texture* texture) const
{
    SDL_Point size;
    SDL_QueryTexture(texture, nullptr, nullptr, &size.x, &size.y);
    return size;
}

SDL_FPoint RenderSystem::getTextureSizeF(const SDL_Point& textureSize) const
{
    return SDL_FPoint{ static_cast<float>(textureSize.x),
            static_cast<float>(textureSize.y) };
}

SDL_Point RenderSystem::getScreenSize() const
{
    SDL_Point s;
    SDL_GetRendererOutputSize(renderer_, &s.x, &s.y);
    return s;
}

SDL_FPoint RenderSystem::getScreenSizeF() const
{
    const SDL_Point s{getScreenSize()};
    return SDL_FPoint{static_cast<float>(s.x),
        static_cast<float>(s.y)};
}

void RenderSystem::initTexturesDescriptors()
{
    std::ifstream descriptorsFile("Data/texturesDescriptors.bin", std::ios::binary);
    if (!descriptorsFile.is_open())
    {
        showMessageBox(__FUNCTION__, "texturesDescriptors.bin is missing!");
        throw std::exception{};
    }

    TextureDescriptor desc{};
    while (descriptorsFile.read(reinterpret_cast<char*>(&desc), sizeof(TextureDescriptor)))
    {
        textureDescriptors_.push_back(desc);
    }

    descriptorsFile.close();
    textures_.reserve(textureDescriptors_.size());
}

Texture RenderSystem::createTexture(const TextureType type) const
{
    const auto& textureRawData{ getTextureData(type) };
    SDL_Texture* texture{ createTextureFromData(textureRawData) };
    const SDL_Point textureSize{ getTextureSize(texture) };

    return Texture{.type = type, .texture = texture, .size = textureSize};
}

std::vector<char> RenderSystem::getTextureData(const TextureType type) const
{
    const auto textureTypeTnt{ static_cast<uint32_t>(type) };
    const auto iter{ std::ranges::find_if(textureDescriptors_,
        [textureTypeTnt](const TextureDescriptor& desc) { return desc.id == textureTypeTnt; }) };
    if (iter == textureDescriptors_.end())
    {
        std::stringstream ss;
        ss << "Unknown TextureType " << textureTypeTnt << "!";
        showMessageBox(__FUNCTION__, ss.str().c_str());
        throw std::exception{};
    }

    std::ifstream texturesFile("Data/textures.bin", std::ios::binary);
    if (!texturesFile.is_open())
    {
        showMessageBox(__FUNCTION__, "textures.bin is missing!");
        throw std::exception{};
    }

    texturesFile.seekg(iter->position, std::ios::beg);
    if (!texturesFile)
    {
        showMessageBox(__FUNCTION__, "textures.bin data is corrupted!");
        throw std::exception{};
    }

    std::vector<char> buffer(iter->size);
    texturesFile.read(buffer.data(), iter->size);
    if (!texturesFile)
    {
        showMessageBox(__FUNCTION__, "textures.bin data is corrupted!");
        throw std::exception{};
    }

    texturesFile.close();
    return buffer;
}

void RenderSystem::processSpriteData(World& world)
{
    const GameStateType gameState{ gameplayStatics::getCurrentGameState(world) };
    if (gameState != GameStateType::INGAME)
    {
        return;
    }
    
    std::array<std::vector<SpriteComponent*>,
        static_cast<size_t>(SpriteLayer::COUNT)> spritesToRender;

    const auto& playerComponent{ gameplayStatics::getPlayerComponent(world)};
    const Entity playerEntity{playerComponent.entity};

    const SDL_FPoint screenSize{ getScreenSizeF() };
    const SDL_FPoint halfScreenSize{.x = screenSize.x / 2.0f,
        .y = screenSize.y / 2.0f};

    const auto& playerTransform{ *world.tryGetComponent<ComponentType::Transform>(playerEntity) };
    SpriteComponent* playerSprite{ processPlayerData(world, halfScreenSize, playerEntity, playerTransform) };
    spritesToRender[static_cast<size_t>(playerSprite->layer)].emplace_back(playerSprite);

    const SDL_FRect cameraRect{
        .x = playerTransform.location.x - halfScreenSize.x,
        .y = playerTransform.location.y - halfScreenSize.y,
        .w = screenSize.x,
        .h = screenSize.y
    };

    for (auto& spriteComponent : world.getComponents<ComponentType::Sprite>())
    {
        if (spriteComponent.entity == playerEntity)
        {
            continue;
        }

        auto& sprite{spriteComponent.instance};

        const SDL_FPoint textureSize{
            getTextureSizeF(sprite.renderData.textureSize)
        };

        const auto& renderTransform{ *world.tryGetComponent<ComponentType::Transform>(spriteComponent.entity) };
        const SDL_FRect renderRect{
            .x = renderTransform.location.x - textureSize.x / 2.0f,
            .y = renderTransform.location.y - textureSize.y / 2.0f,
            .w = textureSize.x,
            .h = textureSize.y
        };

        SDL_FRect intersectRect{};
        if (!SDL_IntersectFRect(&cameraRect, &renderRect, &intersectRect))
        {
            continue;
        }

        auto& layerData{ spritesToRender[static_cast<size_t>(sprite.layer)] };
        layerData.emplace_back(&sprite);
        sprite.renderData.rotation = renderTransform.rotation;

        sprite.renderData.sourceRect = utils::makeRect(
            SDL_Point{
                .x = std::abs(static_cast<int>(renderRect.x - intersectRect.x)),
                .y = std::abs(static_cast<int>(renderRect.y - intersectRect.y))
            },
            SDL_Point{
                .x = static_cast<int>(intersectRect.w),
                .y = static_cast<int>(intersectRect.h)
            }
        );

        sprite.renderData.destinationRect = createDestinationRect(
            intersectRect.x + halfScreenSize.x - playerTransform.location.x,
            intersectRect.y + halfScreenSize.y - playerTransform.location.y,
            intersectRect.w, intersectRect.h
        );
    }

    for (const auto& layer : spritesToRender)
    {
        for (const auto& sprite : layer)
        {
            SDL_RenderCopyExF(renderer_,
                sprite->renderData.texture,
                &sprite->renderData.sourceRect,
                &sprite->renderData.destinationRect,
                sprite->renderData.rotation,
                nullptr, SDL_FLIP_NONE);
        }
    }
}

SpriteComponent* RenderSystem::processPlayerData(World& w, const SDL_FPoint& half_screen_size,
    const Entity playerEntity, const TransformComponent& playerTransform)
{
    auto& sprite{ *w.tryGetComponent<ComponentType::Sprite>(playerEntity) };
    sprite.renderData.rotation = playerTransform.rotation;

    sprite.renderData.sourceRect = utils::makeRect(constants::sdlZeroPoint, sprite.renderData.textureSize);

    sprite.renderData.destinationRect = createDestinationRect(
        half_screen_size.x - sprite.renderData.textureSize.x / 2,
        half_screen_size.y - sprite.renderData.textureSize.y / 2,
        sprite.renderData.sourceRect.w,
        sprite.renderData.sourceRect.h
        );

    return &sprite;
}

void RenderSystem::processWidgetData(World& world)
{
    std::array<std::vector<WidgetComponent*>,
        static_cast<size_t>(WidgetLayer::COUNT)> widgetsToRender;

    for (auto& widgetComponent : world.getComponents<ComponentType::Widget>())
    {
        auto& layerData{ widgetsToRender[static_cast<size_t>(widgetComponent.instance.getLayer())] };
        layerData.emplace_back(&widgetComponent.instance);
    }

    for (const auto& layer : widgetsToRender)
    {
        for (const auto& widget : layer)
        {
            for (const auto& renderData : widget->gatherRenderData())
            {
                SDL_RenderCopyExF(renderer_,
                    renderData->texture,
                    nullptr,
                    &renderData->destinationRect,
                    renderData->rotation,
                    nullptr, SDL_FLIP_NONE);
            }
        }
    }
}

SDL_FRect RenderSystem::createDestinationRect(const float x, const float y, const float w, const float h)
{
    return SDL_FRect{ .x = roundf(x), .y = ceilf(y),
        .w = ceilf(w), .h = ceilf(h) };
}
