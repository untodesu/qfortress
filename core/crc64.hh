#ifndef CORE_CRC64_HH
#define CORE_CRC64_HH
#pragma once

namespace CRC64
{
std::uint64_t calculate(const void* buffer, std::size_t size, std::uint64_t start = 0U);
std::uint64_t calculate(const std::vector<std::byte>& buffer, std::uint64_t start = 0U);
std::uint64_t calculate(const std::string& buffer, std::uint64_t start = 0U);
} // namespace CRC64

#endif // CORE_CRC64_HH
