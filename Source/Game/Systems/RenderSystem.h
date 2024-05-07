#pragma once
#include <cstdint>
#include <vector>
#include <array>
#include "../Generated/TextureType.h"
#include "../Types/Entity.h"
#include "../Components/RenderComponent.h"
#include "../Components/UIRenderComponent.h"
#include "SDL.h"

class World;
class SystemsManager;
struct TransformComponent;

class RenderSystem
{

using RenderData = std::array<std::vector<RenderComponent*>, static_cast<size_t>(RenderLayer::COUNT)>;
using UIRenderData = std::array<std::vector<UIRenderComponent*>, static_cast<size_t>(UIRenderLayer::COUNT)>;

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
	void preinit(World& w, SystemsManager& sm);
	void init(World& w, SystemsManager& sm) {}
	void update(World& w);
	void shutdown();

	void showMessageBox(const char* title, const char* message) const;

    const Texture& getTexture(const TextureType type);
    SDL_Texture* createTexture(const std::vector<char>& data) const;
    SDL_Point getTextureSize(SDL_Texture* texture) const;
    SDL_Point getScreenSize() const;

private:
    void initTexturesDescriptors();
    std::vector<char> getTextureData(const TextureType type) const;
    RenderData gatherRenderData(World& w, const Entity player_ent) const;
    UIRenderData gatherUIRenderData(World& w) const;

    void processPlayerData(World& w, RenderData& render_data, const SDL_FPoint& half_screen_size,
        const Entity player_ent, const TransformComponent& player_transform) const;

private:
    SDL_Window* m_window{ nullptr };
    SDL_Renderer* m_renderer{ nullptr };
    std::vector<TextureDescriptor> m_texture_descriptors;
    std::vector<Texture> m_textures;
};
