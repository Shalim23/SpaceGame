#include "RenderSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include "../Types/Exceptions.h"
#include "../Utils.h"
#include "../GameplayStatics.h"
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
    for (const auto& layer : renderData)
    {
        for (const auto& data : layer)
        {
            SDL_RenderCopyExF(renderer_,
                data.texture,
                &data.sourceRect,
                &data.destinationRect,
                data.rotation,
                nullptr, SDL_FLIP_NONE);
        }
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
