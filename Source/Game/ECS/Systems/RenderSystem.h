#pragma once
#include <vector>
#include <functional>
#include "SDL.h"

class World;
class SystemsManager;
class ViewportSystem;

class RenderSystem
{

public:
	void init(World& world, SystemsManager& systemsManager);
	void update(World& world, const double deltaTime){}
	void shutdown();

    void render();
    void createWindow();

    SDL_Texture* createTextureFromData(const std::vector<char>& rawData) const;
    SDL_Texture* createTextureFromSurface(SDL_Surface* surface) const;
    SDL_Surface* createSurface(const std::vector<char>& rawData) const;

    SDL_Point getTextureSize(SDL_Texture* texture) const;
    SDL_FPoint getTextureSizeF(const SDL_Point& textureSize) const;
    SDL_Point getScreenSize() const;
    SDL_FPoint getScreenSizeF() const;

#ifndef NDEBUG
    void registerDebugUICallback(std::function<void()> callback);
#endif

private:

#ifndef NDEBUG
    void drawDebugUI() const;
#endif

private:
    SDL_Window* window_{ nullptr };
    SDL_Renderer* renderer_{ nullptr };
    ViewportSystem* viewportSystem_{nullptr};

#ifndef NDEBUG
    std::vector<std::function<void()>> debugUICallbacks_;
#endif
};
