#pragma once
#include <random>

class World;
class SystemsManager;
struct SDL_Point;
struct SDL_FPoint;
struct SDL_Color;

class RandomSystem
{
public:
	void preInit(World& world, SystemsManager& systemsManager) {}
	void init(World& world, SystemsManager& systemsManager);
	void postInit(World& world, SystemsManager& systemsManager) {}
	void update(World& world, const double deltaTime) {}
	void shutdown() {}

    int getRandomValue(const SDL_Point& range);
    float getRandomValue(const SDL_FPoint& range);
	SDL_Color getRandomColor();

private:
    std::mt19937 generator_;
};