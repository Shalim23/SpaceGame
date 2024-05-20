#include "Game/Game.h"

int main(int argc, char* args[])
{
    Game g{};
    g.init();
    g.run();
    g.shutdown();

    return 0;
}

// render system - create 2 windows for widget editor mode

//widget builder:
// - add debug configuration
// - add class 
// - design file format
// - binarize
// - read by ui system
// - create widgets from data(make possibility to inject behavior,
//                            e.g. on button click, on hover etc.)