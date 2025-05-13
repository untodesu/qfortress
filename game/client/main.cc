#include "client/pch.hh"

#include "core/cmdline.hh"
#include "core/logging.hh"

int main(int argc, char** argv)
{
    cmdline::init(argc, argv);
    logging::init();

    logging::info("Hello world!");

    logging::deinit();

    return 0;
}
