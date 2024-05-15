#include "Game/Game.h"

int main(int argc, char* args[])
{
    std::unique_ptr<Game> g{std::make_unique<Game>()};
    g->run();

    return 0;
}
