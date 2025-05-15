#include "core/pch.hh"

#include "core/setup.hh"

#include "core/cmdline.hh"
#include "core/version.hh"

void qf_core::setup(int argc, char** argv)
{
    auto logger = spdlog::default_logger();
    auto& logger_sinks = logger->sinks();

    logger_sinks.clear();
    logger_sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());

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

    cmdline::init(argc, argv);

    spdlog::info("qf_core: project version {}", PROJECT_VERSION_STRING);
}

void qf_core::desetup(void)
{
}
