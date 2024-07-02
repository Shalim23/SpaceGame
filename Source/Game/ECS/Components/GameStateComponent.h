#pragma once

enum class GameStateType
{
    NONE,
    MAIN_MENU,
    INGAME,
    INGAME_MENU,
    GAME_OVER
};

struct GameStateComponent
{
    GameStateType gameState{GameStateType::NONE};
};