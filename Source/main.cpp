#include "Game/Game.h"

int main(int argc, char* args[])
{
    Game g{};
    g.init();
    g.run();
    g.shutdown();

    return 0;
}
