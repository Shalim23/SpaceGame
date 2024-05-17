#pragma once
#include "SDL.h"
#include <optional>
#include <vector>

class Input
{
public:
    void clear();
    void processEvent(const SDL_Event& event_);
    std::optional<bool> getKeyState(const SDL_Scancode key) const;

private:
   std::vector<std::pair<SDL_Scancode, bool>> currentKeysState_;
};
