#include "qfmc/pch.hh"

#include "core/setup.hh"

#include "game/shared/game_info.hh"

#include "qfmc/geometry.hh"
#include "qfmc/mapfile.hh"

int main(int argc, char** argv)
{
    if(argc < 2) {
        spdlog::critical("usage: {} <mapfile> [parameters]", argv[0]);
        std::terminate();
    }

    qf_core::setup(argc - 1, argv + 1);

    spdlog::info("QFortress Map Compiler ({})", argv[0]);

    game_info::init();

    mapfile::load(argv[1]);

    geometry::build();

    qf_core::desetup();

    return 0;
}
