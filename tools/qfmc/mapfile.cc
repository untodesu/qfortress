#include "qfmc/pch.hh"

#include "qfmc/mapfile.hh"

#include "core/strtools.hh"

enum class ParserState {
    Global, ///< No active entity
    Entity, ///< Has an active entity, parsing key-value data
    Brush,  ///< Has an active entity, parsing brush data
};

constexpr static int VALVE_MAPVERSION_VALUE = 220;

std::vector<MAP_Entity> mapfile::entities;
const MAP_Entity* mapfile::worldspawn;

static void createMapEntity_worldspawn(const MAP_Entity& entity)
{
    if(mapfile::worldspawn) {
        spdlog::critical("mapfile: multiple worldspawn entities are not allowed");
        std::terminate();
    }

    const auto mapversion = entity.kv.find("mapversion");

    if(mapversion == entity.kv.cend()) {
        spdlog::critical("mapfile: cannot figure out map format without a 'mapversion' property in the worldspawn's KV");
        std::terminate();
    }

    auto mapversion_value = std::atoi(mapversion->second.c_str());

    if(mapversion_value != VALVE_MAPVERSION_VALUE) {
        spdlog::critical("mapfile: unknown map revision {}", mapversion_value);
        std::terminate();
    }

    mapfile::entities.push_back(entity);
    mapfile::worldspawn = &mapfile::entities.back();
}

static void createMapEntity(const MAP_Entity& entity)
{
    if(entity.class_name == "worldspawn") {
        createMapEntity_worldspawn(entity);
        return;
    }

    mapfile::entities.push_back(entity);
}

void mapfile::load(const std::filesystem::path& path)
{
    std::string line;
    std::ifstream file(path, std::ios::in);

    if(!file.is_open()) {
        spdlog::critical("mapfile: {}: cannot open", path.string());
        std::terminate();
    }

    auto current_line = 0UL;
    auto current_state = ParserState::Global;
    MAP_Entity current_entity;
    MAP_Brush current_brush;

    while(std::getline(file, line)) {
        line = strtools::trimWhitespace(line);

        auto comment = line.find_first_of("//");

        if(comment != std::string::npos) {
            line = strtools::trimWhitespace(line.substr(0, comment));
        }

        if(strtools::isWhitespace(line)) {
            current_line += 1;
            continue;
        }

        if(line == "{") {
            if(current_state == ParserState::Global) {
                current_state = ParserState::Entity;
                current_entity.class_name.clear();
                current_entity.geometry.clear();
                current_entity.kv.clear();
                current_line += 1;
                continue;
            }

            if(current_state == ParserState::Entity) {
                current_state = ParserState::Brush;
                current_brush.clear();
                current_line += 1;
                continue;
            }

            spdlog::critical("mapfile: {}:{}: spurious opening brace", path.string(), current_line);
            std::terminate();
        }

        if(line == "}") {
            if(current_state == ParserState::Brush) {
                if(current_brush.empty()) {
                    spdlog::critical("mapfile: {}:{}: the brush has no planes", path.string(), current_line);
                    std::terminate();
                }

                current_state = ParserState::Entity;
                current_entity.geometry.push_back(current_brush);
                current_line += 1;
                continue;
            }

            if(current_state == ParserState::Entity) {
                createMapEntity(current_entity);
                current_state = ParserState::Global;
                current_line += 1;
                continue;
            }

            spdlog::critical("mapfile: {}:{}: spurious closing brace", path.string(), current_line);
            std::terminate();
        }

        if(current_state == ParserState::Entity) {
            static char key_buffer[2048];
            static char value_buffer[2048];

            std::memset(key_buffer, 0, sizeof(key_buffer));
            std::memset(value_buffer, 0, sizeof(value_buffer));

            if(2 == std::sscanf(line.c_str(), "\"%2047[^\"]\" \"%2047[^\"]\"", key_buffer, value_buffer)) {
                if(0 == std::strcmp(key_buffer, "classname")) {
                    current_entity.class_name = value_buffer;
                    current_line += 1;
                    continue;
                }

                if(current_entity.kv.contains(key_buffer)) {
                    // This is not a death sentence, it's just that
                    // the most recent value std::getline-wise will be stored
                    spdlog::warn("mapfile: {}:{}: duplicate key '{}'", path.string(), current_line, key_buffer);
                }

                current_entity.kv.insert_or_assign(key_buffer, value_buffer);
                current_line += 1;
                continue;
            }

            spdlog::critical("mapfile: {}:{}: syntax error: {}", path.string(), current_line, line);
            std::terminate();
        }

        if(current_state == ParserState::Brush) {
            static char texturename_buffer[2048];

            std::memset(texturename_buffer, 0, sizeof(texturename_buffer));

            MAP_BrushPlane plane;
            auto plane_sscanf_result =
                std::sscanf(line.c_str(), "( %f %f %f ) ( %f %f %f ) ( %f %f %f ) %2047[^\r\n\t ] [ %f %f %f %f ] [ %f %f %f %f ] %f %f %f",
                    &plane.points[0].x, &plane.points[0].y, &plane.points[0].z, &plane.points[1].x, &plane.points[1].y, &plane.points[1].z,
                    &plane.points[2].x, &plane.points[2].y, &plane.points[2].z, texturename_buffer, &plane.u_projection.x,
                    &plane.u_projection.y, &plane.u_projection.z, &plane.u_projection.w, &plane.v_projection.x, &plane.v_projection.y,
                    &plane.v_projection.z, &plane.v_projection.w, &plane.rotation, &plane.uv_scale.x, &plane.uv_scale.y);

            if(plane_sscanf_result == 21) {
                plane.texture = texturename_buffer;
                current_brush.push_back(plane);
                current_line += 1;
                continue;
            }

            spdlog::critical("mapfile: {}:{}: syntax error", path.string(), current_line, line);
            std::terminate();
        }

        spdlog::critical("mapfile: {}:{}: if you read this, something very wrong has happened", path.string(), current_line);
        std::terminate();
    }

    if(mapfile::worldspawn == nullptr) {
        spdlog::critical("mapfile: {}: no worldspawn", path.string());
        std::terminate();
    }
}
