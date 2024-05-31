#pragma once
#include "SDL.h"
#include <vector>
#include <functional>

class World;
class SystemsManager;
struct TransformComponent;
struct Input;

class InputSystem
{

struct RegisteredKeyboardInput
{
	SDL_Scancode key;
	bool isOneTimeAction{true};
	bool isPressed{false};
	std::function<void(World&, const double)> onPressed;
	std::function<void(World&, const double)> onReleased;
};

public:
	void init(World& world, SystemsManager& systemsManager);
	void update(World& world, const double deltaTime){}
	void shutdown();
	void processInput(const Input& currentInput, World& world, const double deltaTime);

	void showMouseCursor() const;
	void hideMouseCursor() const;

private:
	void registerInput();

	void processRotation(World& world, const double delteTime) const;

	SDL_FPoint calculateForwardVector(const double rotation) const;
	bool isInGame(World& world) const;

private:
	std::vector<RegisteredKeyboardInput> registeredKeyboardInput_;
};
