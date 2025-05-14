#ifndef SHARED_GAME_INFO_HH
#define SHARED_GAME_INFO_HH
#pragma once

namespace game_info
{
extern std::string game;
extern std::string title;
extern std::string developer;
} // namespace game_info

namespace game_info
{
extern std::filesystem::path data;
extern std::filesystem::path conf;
extern std::filesystem::path path;
} // namespace game_info

namespace game_info
{
void init(void);
} // namespace game_info

#endif // SHARED_GAME_INFO_HH
