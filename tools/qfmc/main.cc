#include "qfmc/pch.hh"

#include "core/setup.hh"

#include "game/shared/game_info.hh"

int main(int argc, char** argv)
{
    qf_core::setup(argc, argv);

    spdlog::info("QFortress Map Compiler ({})", argv[0]);

    game_info::init();

    spdlog::info("Hello world!");

    qf_core::desetup();

    return 0;
}
