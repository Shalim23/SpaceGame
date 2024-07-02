#include "Input.h"

void Input::clear()
{
    currentKeysState_.clear();
}

void Input::processEvent(const SDL_Event& event_)
{
    switch (event_.type)
    {
    case SDL_KEYDOWN:
    {
        if (!event_.key.repeat)
        {
            currentKeysState_.push_back(std::make_pair(event_.key.keysym.scancode, true));
        }
        break;
    }
    case SDL_KEYUP:
    {
        if (!event_.key.repeat)
        {
            currentKeysState_.push_back(std::make_pair(event_.key.keysym.scancode, false));
        }
        break;
    }
    }
}

std::optional<bool> Input::getKeyState(const SDL_Scancode key) const
{
    const auto iter{std::ranges::find_if(currentKeysState_,
        [key](const auto& state){ return state.first == key; })};
    return iter != currentKeysState_.end() ? std::optional{iter->second} : std::nullopt;
}
