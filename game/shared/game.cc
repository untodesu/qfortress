#include "shared/pch.hh"

#include "shared/game.hh"

#include "core/cmdline.hh"
#include "core/config.hh"
#include "core/logging.hh"
#include "core/version.hh"

#include "shared/game_info.hh"
#include "shared/globals.hh"

void game_shared::init(int argc, char** argv)
{
    cmdline::init(argc, argv);

    logging::init();
    logging::info("Project version: {}", PROJECT_VERSION_STRING);

    game_info::init();

    globals::config.loadFile(game_info::conf / "config.conf");
    globals::config.loadFile(game_info::conf / "custom.conf");
}

void game_shared::deinit(void)
{
    globals::config.saveFile(game_info::conf / "config.conf");

    logging::deinit();
}
