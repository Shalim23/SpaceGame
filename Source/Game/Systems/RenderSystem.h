#pragma once
#include <cstdint>
#include <vector>
#include <array>
#include "../Generated/TextureType.h"
#include "../Types/Entity.h"
#include "SDL.h"

class World;
class SystemsManager;
class ViewportSystem;
struct TransformComponent;
struct SpriteComponent;

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

private:
    
    
    
    void processSpriteData(World& world);

    SpriteComponent* processPlayerData(World& world, const SDL_FPoint& halfScreenSize,
        const Entity playerEntity, const TransformComponent& playerTransform);

    

    SDL_FRect createDestinationRect(const float x, const float y,
        const float w, const float h);

private:
    SDL_Window* window_{ nullptr };
    SDL_Renderer* renderer_{ nullptr };

    ViewportSystem* viewportSystem_{nullptr};
};
