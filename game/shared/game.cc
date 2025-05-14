#include "shared/pch.hh"

#include "shared/game.hh"

#include "core/cmdline.hh"
#include "core/config.hh"
#include "core/version.hh"

#include "shared/game_info.hh"
#include "shared/globals.hh"

void game_shared::init(int argc, char** argv)
{
    auto logger = spdlog::default_logger();
    auto& logger_sinks = logger->sinks();

    logger_sinks.clear();
    logger_sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logger_sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("qf.log", true));

#if defined(NDEBUG)
    constexpr auto default_loglevel = spdlog::level::info;
#else
    constexpr auto default_loglevel = spdlog::level::trace;
#endif

    if(cmdline::contains("quiet")) {
        logger->set_level(spdlog::level::warn);
    }
    else if(cmdline::contains("verbose")) {
        logger->set_level(spdlog::level::trace);
    }
    else {
        logger->set_level(default_loglevel);
    }

    logger->set_pattern("%H:%M:%S.%e %^[%L]%$ %v");
    logger->flush();

    spdlog::info("qf: project version: {}", PROJECT_VERSION_STRING);

    cmdline::init(argc, argv);

    game_info::init();

    globals::config.loadFile(game_info::conf / "config.conf");
    globals::config.loadFile(game_info::conf / "custom.conf");
}

void game_shared::deinit(void)
{
    globals::config.saveFile(game_info::conf / "config.conf");
}
