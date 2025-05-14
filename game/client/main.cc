#include "client/pch.hh"

#include "shared/game.hh"

int main(int argc, char** argv)
{
    game_shared::init(argc, argv);

    game_shared::deinit();

    return 0;
}
