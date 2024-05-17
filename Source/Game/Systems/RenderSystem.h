#pragma once
#include <cstdint>
#include <vector>
#include <array>
#include "../Generated/TextureType.h"
#include "../Types/Entity.h"
#include "../Types/Texture.h"
#include "SDL.h"

class World;
class SystemsManager;
struct TransformComponent;
struct SpriteComponent;

class RenderSystem
{

private:
    struct TextureDescriptor
    {
        uint32_t id;
        uint32_t position;
        uint32_t size;
    };

public:
    void preInit(World& world, SystemsManager& systemsManager);
	void init(World& world, SystemsManager& systemsManager){}
	void postInit(World& world, SystemsManager& systemsManager){}
	void update(World& world);
	void shutdown();

	void showMessageBox(const char* title, const char* message) const;

    const Texture& getTexture(const TextureType type);
    Texture createDynamicTexture(const TextureType type, const Entity entity);
    void addDynamicTexture(const Entity entity, SDL_Texture* texture);
    SDL_Texture* createTextureFromSurface(SDL_Surface* surface) const;
    SDL_Surface* createSurface(const TextureType type) const;

    SDL_Point getTextureSize(SDL_Texture* texture) const;
    SDL_FPoint getTextureSizeF(const SDL_Point& textureSize) const;
    SDL_Point getScreenSize() const;
    SDL_FPoint getScreenSizeF() const;

private:
    void initTexturesDescriptors();
    Texture createTexture(const TextureType type) const;
    SDL_Texture* createTextureFromData(const std::vector<char>& rawData) const;
    std::vector<char> getTextureData(const TextureType type) const;
    void processSpriteData(World& world);

    SpriteComponent* processPlayerData(World& world, const SDL_FPoint& halfScreenSize,
        const Entity playerEntity, const TransformComponent& playerTransform);

    void processWidgetData(World& world);

    SDL_FRect createDestinationRect(const float x, const float y,
        const float w, const float h);

private:
    SDL_Window* window_{ nullptr };
    SDL_Renderer* renderer_{ nullptr };
    std::vector<TextureDescriptor> textureDescriptors_;
    std::vector<Texture> textures_;
    std::vector<DynamicTexture> dynamicTextures_;
};
