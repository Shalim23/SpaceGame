#include "InputSystem.h"
#include "../World.h"
#include "../SystemsManager.h"
#include "../Constants.h"
#include "../GameplayStatics.h"
#include "../Types/Input.h"
#include <numbers>

namespace
{
    constexpr double rotationRatePerSecond{ 100.0 };
    constexpr float movementSpeedPerSecond{ 250.0 };
}

void InputSystem::init(World& world, SystemsManager& systemsManager)
{
    const auto& dbSystem{systemsManager.getSystem<DatabaseSystem>()};
    SDL_Surface* cursorImage{ dbSystem.createSurface(TextureType::UI_cursor)};
    SDL_Cursor* cursor{SDL_CreateColorCursor(cursorImage, 0, 0)};
    SDL_SetCursor(cursor);
    SDL_FreeSurface(cursorImage);

    registerInput();
}

void InputSystem::shutdown()
{
    SDL_FreeCursor(SDL_GetCursor());
}

void InputSystem::processInput(const Input& currentInput, World& world, const double deltaTime)
{
    for (auto& registeredInput : registeredKeyboardInput_)
    {
        const auto state{currentInput.getKeyState(registeredInput.key)};
        if (state != std::nullopt)
        {
            if (state.value())
            {
                if(registeredInput.onPressed)
                {
                    if(registeredInput.isOneTimeAction)
                    {
                        registeredInput.onPressed(world, deltaTime);
                    }
                    else
                    {
                        registeredInput.isPressed = true;
                    }
                }
            }
            else
            {
                if (registeredInput.onReleased)
                {
                    registeredInput.onReleased(world, deltaTime);
                }
                if (!registeredInput.isOneTimeAction)
                {
                    registeredInput.isPressed = false;
                }
            }
        }

        if(registeredInput.isPressed)
        {
            registeredInput.onPressed(world, deltaTime);
        }
    }
}

void InputSystem::showMouseCursor() const
{
    SDL_ShowCursor(SDL_ENABLE);
}

void InputSystem::hideMouseCursor() const
{
    SDL_ShowCursor(SDL_DISABLE);
}

void InputSystem::registerInput()
{
#ifdef WIDGETBUILDER
    //#TODO add refresh input
#else
    registeredKeyboardInput_.emplace_back(
        RegisteredKeyboardInput{
            .key = SDL_SCANCODE_W,
            .isOneTimeAction = false,
            .onPressed = [this](World& world, const double)
            {
                if (!isInGame(world))
                {
                    return;
                }
                
                const auto& playerComponent{ gameplayStatics::getPlayerComponent(world) };
                const auto& transform{ *world.tryGetComponent<TransformComponent>(playerComponent.entity) };

                auto& movement{ *world.tryGetComponent<MovementComponent>(playerComponent.entity) };
                movement.forwardVector = calculateForwardVector(transform.rotation);
                movement.speedPerSecond = movementSpeedPerSecond;
            },
            .onReleased = [this](World& world, const double)
            {
                if(!isInGame(world))
                {
                    return;
                }
                
                const auto& playerComponent{ gameplayStatics::getPlayerComponent(world) };
                auto& movement{ *world.tryGetComponent<MovementComponent>(playerComponent.entity) };
                movement.speedPerSecond = float{};
            }
        });

    registeredKeyboardInput_.emplace_back(
        RegisteredKeyboardInput{
            .key = SDL_SCANCODE_A,
            .isOneTimeAction = false,
            .onPressed = [this](World& world, const double deltaTime)
            {
                processRotation(world, -deltaTime);
            }
        });

    registeredKeyboardInput_.emplace_back(
        RegisteredKeyboardInput{
            .key = SDL_SCANCODE_D,
            .isOneTimeAction = false,
            .onPressed = [this](World& world, const double deltaTime)
            {
                processRotation(world, deltaTime);
            }
        });
#endif
}

void InputSystem::processRotation(World& world, const double deltaTime) const
{
    if (!isInGame(world))
    {
        return;
    }
    
    const auto& playerComponent{ gameplayStatics::getPlayerComponent(world) };
    auto& transform{ *world.tryGetComponent<TransformComponent>(playerComponent.entity) };
    
    const double rotationDelta{ rotationRatePerSecond * deltaTime };
    transform.rotation += rotationDelta;

    constexpr double epsilon{ std::numeric_limits<double>::epsilon() };
    while (transform.rotation < 0.0 - epsilon)
    {
        transform.rotation += constants::fullCircleDegreesD;
    }
    while (transform.rotation >= constants::fullCircleDegreesD + epsilon)
    {
        transform.rotation -= constants::fullCircleDegreesD;
    }
}

SDL_FPoint InputSystem::calculateForwardVector(const double rotation) const
{
    const float radians{static_cast<float>(rotation * std::numbers::pi
        / (constants::fullCircleDegreesD / 2.0))};
    return SDL_FPoint{.x = sin(radians), .y = cos(radians) * -1.0f };
}

bool InputSystem::isInGame(World& world) const
{
    const GameStateType gameState{ gameplayStatics::getCurrentGameState(world) };
    return gameState == GameStateType::INGAME;
}
