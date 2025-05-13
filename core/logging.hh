#ifndef CORE_LOGGING_HH
#define CORE_LOGGING_HH
#pragma once

namespace logging
{
void init(void);
void deinit(void);
} // namespace logging

namespace logging
{
void info(const std::string& message);
void warn(const std::string& message);
void error(const std::string& message);
} // namespace logging

namespace logging
{
template<typename... Args>
inline void info(const std::format_string<Args...> fmt, Args&&... args);
template<typename... Args>
inline void warn(const std::format_string<Args...> fmt, Args&&... args);
template<typename... Args>
inline void error(const std::format_string<Args...> fmt, Args&&... args);
} // namespace logging

// ============================================================================
// Inline methods implementations go after this comment
// ============================================================================

template<typename... Args>
inline void logging::info(const std::format_string<Args...> fmt, Args&&... args)
{
    logging::info(std::vformat(fmt.get(), std::make_format_args(args...)));
}

template<typename... Args>
inline void logging::warn(const std::format_string<Args...> fmt, Args&&... args)
{
    logging::warn(std::vformat(fmt.get(), std::make_format_args(args...)));
}

template<typename... Args>
inline void logging::error(const std::format_string<Args...> fmt, Args&&... args)
{
    logging::error(std::vformat(fmt.get(), std::make_format_args(args...)));
}

#endif // CORE_LOGGING_HH
