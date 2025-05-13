#ifndef CORE_EPOCH_HH
#define CORE_EPOCH_HH
#pragma once

namespace epoch
{
std::uint64_t seconds(void);
std::uint64_t milliseconds(void);
std::uint64_t microseconds(void);
} // namespace epoch

namespace epoch
{
std::int64_t signedSeconds(void);
std::int64_t signedMilliseconds(void);
std::int64_t signedMicroseconds(void);
} // namespace epoch

#endif // CORE_EPOCH_HH
