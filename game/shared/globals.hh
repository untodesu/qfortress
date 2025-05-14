#ifndef SHARED_GLOBALS_HH
#define SHARED_GLOBALS_HH
#pragma once

class Config;

namespace globals
{
extern entt::dispatcher dispatcher;
extern entt::registry registry;
} // namespace globals

namespace globals
{
extern float fixed_frametime;
extern float fixed_frametime_avg;
extern std::uint64_t fixed_frametime_us;
extern std::size_t fixed_framecount;
} // namespace globals

namespace globals
{
extern std::uint64_t curtime;
} // namespace globals

namespace globals
{
extern Config config;
} // namespace globals

#endif // SHARED_GLOBALS_HH
