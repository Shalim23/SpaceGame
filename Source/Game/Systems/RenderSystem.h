#pragma once
#include <cstdint>
#include <vector>
#include <array>
#include "../Generated/TextureType.h"
#include "../Types/Entity.h"
#include "../Components/SpriteComponent.h"
#include "SDL.h"

class World;
class SystemsManager;
struct TransformComponent;

class RenderSystem
{

using SpritesToRender = std::array<std::vector<SpriteComponent*>, static_cast<size_t>(SpriteLayer::COUNT)>;

public:
    struct Texture
    {
        TextureType type;
        SDL_Texture* texture;
        SDL_Point size;
    };

private:
    struct TextureDescriptor
    {
        uint32_t id;
        uint32_t position;
        uint32_t size;
    };


public:
	void preInit(World& w, SystemsManager& sm);
	void init(World& w, SystemsManager& sm) {}
	void update(World& w);
	void shutdown();

	void showMessageBox(const char* title, const char* message) const;

    const Texture& getTexture(const TextureType type);
    SDL_Texture* createTexture(const std::vector<char>& data) const;
    SDL_Point getTextureSize(SDL_Texture* texture) const;
    SDL_FPoint getTextureSizeF(const SDL_Point& texture_size) const;
    SDL_Point getScreenSize() const;
    SDL_FPoint getScreenSizeF() const;

private:
    void initTexturesDescriptors();
    std::vector<char> getTextureData(const TextureType type) const;
    void processRenderData(World& w, const Entity player_ent);

    void processPlayerData(World& w, const SDL_FPoint& half_screen_size,
        const Entity player_ent, const TransformComponent& player_transform);

    SDL_Rect createSourceRect(const int x, const int y,
        const int w, const int h);
    SDL_FRect createDestinationRect(const float x, const float y,
        const float w, const float h);

private:
    SDL_Window* m_window{ nullptr };
    SDL_Renderer* m_renderer{ nullptr };
    std::vector<TextureDescriptor> m_texture_descriptors;
    std::vector<Texture> m_textures;
    SpritesToRender m_sprites_to_render;
};
