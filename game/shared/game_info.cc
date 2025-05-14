#include "shared/pch.hh"

#include "shared/game_info.hh"

#include "core/cmdline.hh"
#include "core/logging.hh"

constexpr static const char* DEFAULT_GAME_NAME = "QFortress";
constexpr static const char* DEFAULT_GAME_TITLE = "QFortress";
constexpr static const char* DEFAULT_GAME_DEVELOPER = "untodesu";

constexpr static const char* GAMEINFO_JSON = "gameinfo.json";

constexpr static const char* SUBDIR_DATA = "data";
constexpr static const char* SUBDIR_CONF = "conf";

std::string game_info::game;
std::string game_info::title;
std::string game_info::developer;

std::filesystem::path game_info::data;
std::filesystem::path game_info::conf;
std::filesystem::path game_info::path;

void game_info::init(void)
{
    auto game_dir = cmdline::getArgument("game");
    game_info::path = game_dir ? game_dir : std::filesystem::current_path();
    game_info::data = game_info::path / SUBDIR_DATA;
    game_info::conf = game_info::path / SUBDIR_CONF;

    if(!std::filesystem::exists(game_info::path)) {
        logging::error("game_info: unable to locate game directory at {}", game_info::path.string());
        std::terminate();
    }

    auto json_path = game_info::path / GAMEINFO_JSON;
    auto json_value = json_parse_file(json_path.string().c_str());

    if(json_value == nullptr) {
        logging::error("game_info: unable to locate or parse gameinfo.json at {}", game_info::path.string());
        std::terminate();
    }

    auto json_root = json_value_get_object(json_value);
    auto json_game = json_object_get_string(json_root, "game");
    auto json_title = json_object_get_string(json_root, "title");
    auto json_developer = json_object_get_string(json_root, "developer");

    game_info::game = json_game ? json_game : DEFAULT_GAME_NAME;
    game_info::title = json_title ? json_title : DEFAULT_GAME_TITLE;
    game_info::developer = json_developer ? json_developer : DEFAULT_GAME_DEVELOPER;

    json_value_free(json_value);

    std::filesystem::create_directories(game_info::conf);
    std::filesystem::create_directories(game_info::data);
}
