#pragma once
#include <cstdint>

class World;
class SystemsManager;
struct SDL_Window;
struct SDL_Renderer;

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
	void init(World& w, SystemsManager& sm);
	void update(World& w) {}
	void shutdown();

	void showMessageBox(const char* title, const char* message);
    SDL_Texture* createTexture(const std::vector<char>& data) const;
    SDL_Point getTextureSize(SDL_Texture* texture) const;

private:
    SDL_Window* m_window{ nullptr };
    SDL_Renderer* m_renderer{ nullptr };
};
