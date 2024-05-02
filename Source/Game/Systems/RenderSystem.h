#pragma once

class World;
class SystemsManager;
struct SDL_Window;
struct SDL_Renderer;

class RenderSystem
{
public:
	void init(World& w, SystemsManager& sm);
	void update(World& w) {}
	void shutdown();

	void showMessageBox(const char* title, const char* message);

private:
    SDL_Window* m_window{ nullptr };
    SDL_Renderer* m_renderer{ nullptr };
};
