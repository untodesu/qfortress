#include "core/pch.hh"

#include "core/cmdline.hh"
#include "core/logging.hh"

constexpr static const char* PUT_TIME_FORMAT = "%Y-%m-%d %H:%M:%S ";

static std::mutex s_mutex;
static std::ofstream s_logfile;

void logging::init(void)
{
    if(auto filename = cmdline::getArgument("logfile")) {
        s_logfile.open(filename, std::ios::app);
    }
}

void logging::deinit(void)
{
    s_logfile.close();
}

void logging::info(const std::string& message)
{
    auto curtime = std::time(nullptr);
    auto curtime_tm = std::gmtime(&curtime);

    s_mutex.lock();

    if(s_logfile.is_open()) {
        s_logfile << std::put_time(curtime_tm, PUT_TIME_FORMAT);
        s_logfile << "[INFO] ";
        s_logfile << message;
        s_logfile << std::endl;
    }

    if(std::cerr.good()) {
        std::cerr << std::put_time(curtime_tm, PUT_TIME_FORMAT);
        std::cerr << "\033[1;32;92m[INFO]\033[0m ";
        std::cerr << message;
        std::cerr << std::endl;
    }

    s_mutex.unlock();
}

void logging::warn(const std::string& message)
{
    auto curtime = std::time(nullptr);
    auto curtime_tm = std::gmtime(&curtime);

    s_mutex.lock();

    if(s_logfile.is_open()) {
        s_logfile << std::put_time(curtime_tm, PUT_TIME_FORMAT);
        s_logfile << "[WARN] ";
        s_logfile << message;
        s_logfile << std::endl;
    }

    if(std::cerr.good()) {
        std::cerr << std::put_time(curtime_tm, PUT_TIME_FORMAT);
        std::cerr << "\033[1;33;93m[WARN]\033[0m ";
        std::cerr << message;
        std::cerr << std::endl;
    }

    s_mutex.unlock();
}

void logging::error(const std::string& message)
{
    auto curtime = std::time(nullptr);
    auto curtime_tm = std::gmtime(&curtime);

    s_mutex.lock();

    if(s_logfile.is_open()) {
        s_logfile << std::put_time(curtime_tm, PUT_TIME_FORMAT);
        s_logfile << "[ERROR] ";
        s_logfile << message;
        s_logfile << std::endl;
    }

    if(std::cerr.good()) {
        std::cerr << std::put_time(curtime_tm, PUT_TIME_FORMAT);
        std::cerr << "\033[1;31;91m[ERROR]\033[0m ";
        std::cerr << message;
        std::cerr << std::endl;
    }

    s_mutex.unlock();
}
