#pragma once
#include <cstdint>
#include <vector>
#include "../Generated/TextureType.h"
#include "SDL.h"

class World;
class SystemsManager;

class RenderSystem
{

private:
    struct TextureDescriptor
    {
        uint32_t id;
        uint32_t position;
        uint32_t size;
    };

    struct Texture
    {
        explicit Texture(const TextureType t,
            SDL_Texture* tex, const SDL_Point& s)
            : type{ t }, texture{ tex }, size{ s }
        {}
        TextureType type;
        SDL_Texture* texture;
        SDL_Point size;
    };

public:
	void init(World& w, SystemsManager& sm);
	void update(World& w);
	void shutdown();

	void showMessageBox(const char* title, const char* message) const;

    const Texture& getTexture(const TextureType type);
    SDL_Texture* createTexture(const std::vector<char>& data) const;
    SDL_Point getTextureSize(SDL_Texture* texture) const;

private:
    void initTexturesDescriptors();
    std::vector<char> getTextureData(const TextureType type) const;

private:
    SDL_Window* m_window{ nullptr };
    SDL_Renderer* m_renderer{ nullptr };
    std::vector<TextureDescriptor> m_texture_descriptors;
    std::vector<Texture> m_textures;
};
