#ifndef CORE_CMDLINE_HH
#define CORE_CMDLINE_HH
#pragma once

constexpr static char CMDLINE_OPTION_PREFIX = '-';

namespace cmdline
{
void init(int argc, char** argv);
} // namespace cmdline

namespace cmdline
{
void add(std::string_view option);
void add(std::string_view option, std::string_view argument);
} // namespace cmdline

namespace cmdline
{
bool contains(std::string_view option);
} // namespace cmdline

namespace cmdline
{
const char* getArgument(std::string_view option, const char* fallback = nullptr);
} // namespace cmdline

#endif // CORE_CMDLINE_HH
