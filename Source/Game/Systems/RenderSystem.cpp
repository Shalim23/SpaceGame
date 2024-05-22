#include "RenderSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include "../Types/Exceptions.h"
#include "../Utils.h"
#include "../GameplayStatics.h"
#include "../Constants.h"
#include "SDL_image.h"
#include <sstream>

#ifndef NDEBUG
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#endif

void RenderSystem::init(World& world, SystemsManager& systemsManager)
{
    viewportSystem_ = &systemsManager.getSystem<ViewportSystem>();
}

void RenderSystem::shutdown()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);

    renderer_ = nullptr;
    window_ = nullptr;
}

void RenderSystem::render()
{
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer_);

    /*ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::Render();

    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());*/

    const auto& renderData{viewportSystem_->getRenderData()};
    for (const auto& data : renderData)
    {
        SDL_RenderCopyExF(renderer_,
            data.texture,
            &data.sourceRect,
            &data.destinationRect,
            data.rotation,
            nullptr, SDL_FLIP_NONE);
    }

    SDL_RenderPresent(renderer_);
}

void RenderSystem::createWindow()
{
    assert(!window_ && "Window is already created!");
    assert(!renderer_ && "Renderer is already created!");
    
    constexpr Uint32 fullscreenFlag{ 0 };
    //constexpr Uint32 fullscreenFlag{ SDL_WINDOW_FULLSCREEN_DESKTOP };

    window_ = SDL_CreateWindow("Space Game", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 1920, 1080, fullscreenFlag);
    if (!window_)
    {
        utils::showMessageBox(__FUNCTION__, "Failed to create window!");
        throw SystemInitException{};
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer_)
    {
        utils::showMessageBox(__FUNCTION__, "Failed to create renderer!");
        throw SystemInitException{};
    }

#ifndef NDEBUG
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io{ ImGui::GetIO() };

    ImGui_ImplSDL2_InitForSDLRenderer(window_, renderer_);
    ImGui_ImplSDLRenderer2_Init(renderer_);
#endif
}

SDL_Texture* RenderSystem::createTextureFromData(const std::vector<char>& rawData) const
{
    return IMG_LoadTexture_RW(renderer_, SDL_RWFromConstMem(rawData.data(), rawData.size()), 1);
}

SDL_Texture* RenderSystem::createTextureFromSurface(SDL_Surface* surface) const
{
    return SDL_CreateTextureFromSurface(renderer_, surface);
}

SDL_Surface* RenderSystem::createSurface(const std::vector<char>& rawData) const
{
    return IMG_Load_RW(SDL_RWFromConstMem(rawData.data(), rawData.size()), 1);
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

    const auto& playerTransform{ *world.tryGetComponent<TransformComponent>(playerEntity) };
    SpriteComponent* playerSprite{ processPlayerData(world, halfScreenSize, playerEntity, playerTransform) };
    spritesToRender[static_cast<size_t>(playerSprite->layer)].emplace_back(playerSprite);

    const SDL_FRect cameraRect{
        .x = playerTransform.location.x - halfScreenSize.x,
        .y = playerTransform.location.y - halfScreenSize.y,
        .w = screenSize.x,
        .h = screenSize.y
    };

    for (auto& spriteComponent : world.getComponents<SpriteComponent>())
    {
        if (spriteComponent.entity == playerEntity)
        {
            continue;
        }

        auto& sprite{spriteComponent.instance};

        const SDL_FPoint textureSize{
            getTextureSizeF(sprite.renderData.textureSize)
        };

        const auto& renderTransform{ *world.tryGetComponent<TransformComponent>(spriteComponent.entity) };
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
    auto& sprite{ *w.tryGetComponent<SpriteComponent>(playerEntity) };
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

//void RenderSystem::processWidgetData(World& world)
//{
//    
//}

SDL_FRect RenderSystem::createDestinationRect(const float x, const float y, const float w, const float h)
{
    return SDL_FRect{ .x = roundf(x), .y = ceilf(y),
        .w = ceilf(w), .h = ceilf(h) };
}
