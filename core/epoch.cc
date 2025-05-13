#include "core/pch.hh"

#include "core/epoch.hh"

std::uint64_t epoch::seconds(void)
{
    auto current = std::chrono::system_clock::now();
    auto time_since_epoch = current.time_since_epoch();
    return static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::seconds>(time_since_epoch).count());
}

std::uint64_t epoch::milliseconds(void)
{
    auto current = std::chrono::system_clock::now();
    auto time_since_epoch = current.time_since_epoch();
    return static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(time_since_epoch).count());
}

std::uint64_t epoch::microseconds(void)
{
    auto current = std::chrono::system_clock::now();
    auto time_since_epoch = current.time_since_epoch();
    return static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(time_since_epoch).count());
}

std::int64_t epoch::signedSeconds(void)
{
    auto current = std::chrono::system_clock::now();
    auto time_since_epoch = current.time_since_epoch();
    return static_cast<std::int64_t>(std::chrono::duration_cast<std::chrono::seconds>(time_since_epoch).count());
}

std::int64_t epoch::signedMilliseconds(void)
{
    auto current = std::chrono::system_clock::now();
    auto time_since_epoch = current.time_since_epoch();
    return static_cast<std::int64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(time_since_epoch).count());
}

std::int64_t epoch::signedMicroseconds(void)
{
    auto current = std::chrono::system_clock::now();
    auto time_since_epoch = current.time_since_epoch();
    return static_cast<std::int64_t>(std::chrono::duration_cast<std::chrono::microseconds>(time_since_epoch).count());
}
