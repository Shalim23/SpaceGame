#pragma once
#include <cstdint>
#include <vector>
#include <array>
#include "../Generated/TextureType.h"
#include "../Types/Entity.h"
#include "../Types/Texture.h"
#include "../Components/SpriteComponent.h"
#include "../Components/WidgetComponent.h"
#include "SDL.h"

class World;
class SystemsManager;
struct TransformComponent;

class RenderSystem
{

using SpritesToRender =
    std::array<std::vector<SpriteComponent*>,
        static_cast<size_t>(SpriteLayer::COUNT)>;

using WidgetsToRender =
    std::array<std::vector<WidgetComponent*>,
        static_cast<size_t>(WidgetLayer::COUNT)>;

private:
    struct TextureDescriptor
    {
        uint32_t id;
        uint32_t position;
        uint32_t size;
    };


public:
	void init(World& world, SystemsManager& systemsManager);
	void postInit(World& world, SystemsManager& systemsManager){}
	void update(World& world);
	void shutdown();

	void showMessageBox(const char* title, const char* message) const;

    const Texture& getTexture(const TextureType type);
    SDL_Texture* createTexture(const std::vector<char>& rawData) const;
    SDL_Point getTextureSize(SDL_Texture* texture) const;
    SDL_FPoint getTextureSizeF(const SDL_Point& textureSize) const;
    SDL_Point getScreenSize() const;
    SDL_FPoint getScreenSizeF() const;

private:
    void initTexturesDescriptors();
    std::vector<char> getTextureData(const TextureType type) const;
    void processRenderData(World& world, const Entity playerEntity);

    void processPlayerData(World& world, const SDL_FPoint& halfScreenSize,
        const Entity playerEntity, const TransformComponent& playerTransform);

    SDL_Rect createSourceRect(const int x, const int y,
        const int w, const int h);
    SDL_FRect createDestinationRect(const float x, const float y,
        const float w, const float h);

private:
    SDL_Window* window_{ nullptr };
    SDL_Renderer* renderer_{ nullptr };
    std::vector<TextureDescriptor> textureDescriptors_;
    std::vector<Texture> textures_;
    SpritesToRender spritesToRender_;
    WidgetsToRender widgetsToRender_;
};
