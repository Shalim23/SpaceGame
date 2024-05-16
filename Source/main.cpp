#include "Game/Game.h"

int main(int argc, char* args[])
{
    //SDLWrapper{};
    //RenderSystem{};

    
    std::unique_ptr<Game> g{std::make_unique<Game>()};
    g->run();

    return 0;
}


//restructure:
// - separate SDL init
// - separate render(data consume only)
// - game objects render data calculation
// - ui render data calculation

//widget builder:
// - add debug configuration
// - add class 
// - design file format
// - binarize
// - read by ui system
// - create widgets from data(make possibility to inject behavior,
//                            e.g. on button click, on hover etc.)