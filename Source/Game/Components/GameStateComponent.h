#pragma once

enum class GameStateType
{
    IN_MAIN_MENU,
    IN_GAME,
    IN_GAME_MENU,
    GAME_OVER
};

struct GameStateComponent
{
    GameStateType gameState{GameStateType::IN_MAIN_MENU};
};