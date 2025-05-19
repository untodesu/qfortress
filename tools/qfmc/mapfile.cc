#include "qfmc/pch.hh"

#include "qfmc/mapfile.hh"

#include "core/strtools.hh"

using ParserState = unsigned short;
constexpr static ParserState PARSE_GLOBAL = 0U;
constexpr static ParserState PARSE_ENTITY = 1U;
constexpr static ParserState PARSE_BRUSH = 2U;

// I'm pretty sure the code would fail with a syntax
// error when it encounters a brush plane description that
// doesn't exactly comply with Valve220 mapfile format but
// it's still a good idea to cross-reference the mapversion value
constexpr static int VALVE220_MAPVERSION = 220;

std::vector<Mapfile_Entity> mapfile::entities;
const Mapfile_Entity* mapfile::worldspawn = nullptr;

// Specific case for an entity with entity_classname set to worldspawn;
// This validates the internals of the entity and ensures there is only one
// worldspawn entity present in the entire file, otherwise it std::terminate-s
static void createMapEntity_worldspawn(const Mapfile_Entity& entity)
{
    if(mapfile::worldspawn) {
        spdlog::critical("mapfile: spurious second worldspawn entity");
        std::terminate();
    }

    const auto mapversion_kv = entity.kv.find("mapversion");

    if(mapversion_kv == entity.kv.cend()) {
        spdlog::critical("mapfile: cannot validate worldspawn without a 'mapversion' KV entry");
        std::terminate();
    }

    auto mapversion_value = std::atoi(mapversion_kv->second.c_str());

    if(mapversion_value != VALVE220_MAPVERSION) {
        spdlog::critical("mapfile: unknown mapversion value [parsed {} while {} supported]", mapversion_value, VALVE220_MAPVERSION);
        std::terminate();
    }

    mapfile::entities.push_back(entity);
    mapfile::worldspawn = &mapfile::entities.back();
}

static void createMapEntity(const Mapfile_Entity& entity)
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

    auto current_line_no = 0UL;
    auto current_parse = PARSE_GLOBAL;
    Mapfile_Entity current_entity;
    Mapfile_Brush current_brush;

    while(std::getline(file, line)) {
        line = strtools::trimWhitespace(line);

        auto comment = line.find_first_of("//");

        if(comment != std::string::npos) {
            line = strtools::trimWhitespace(line.substr(0, comment));
        }

        if(strtools::isWhitespace(line)) {
            current_line_no += 1;
            continue;
        }

        if(line == "{") {
            if(current_parse == PARSE_GLOBAL) {
                current_parse = PARSE_ENTITY;
                current_entity.class_name.clear();
                current_entity.geometry.clear();
                current_entity.kv.clear();
                current_line_no += 1;
                continue;
            }

            if(current_parse == PARSE_ENTITY) {
                current_parse = PARSE_BRUSH;
                current_brush.planes.clear();
                current_line_no += 1;
                continue;
            }

            spdlog::critical("mapfile: {}:{}: spurious opening brace", path.string(), current_line_no);
            std::terminate();
        }

        if(line == "}") {
            if(current_parse == PARSE_BRUSH) {
                if(current_brush.planes.empty()) {
                    spdlog::critical("mapfile: {}:{}: the brush has no planes", path.string(), current_line_no);
                    std::terminate();
                }

                current_parse = PARSE_ENTITY;
                current_entity.geometry.push_back(current_brush);
                current_line_no += 1;
                continue;
            }

            if(current_parse == PARSE_ENTITY) {
                createMapEntity(current_entity);
                current_parse = PARSE_GLOBAL;
                current_line_no += 1;
                continue;
            }

            spdlog::critical("mapfile: {}:{}: spurious closing brace", path.string(), current_line_no);
            std::terminate();
        }

        if(current_parse == PARSE_ENTITY) {
            static char key_buffer[2048];
            static char value_buffer[2048];

            std::memset(key_buffer, 0, sizeof(key_buffer));
            std::memset(value_buffer, 0, sizeof(value_buffer));

            if(2 == std::sscanf(line.c_str(), "\"%2047[^\"]\" \"%2047[^\"]\"", key_buffer, value_buffer)) {
                if(0 == std::strcmp(key_buffer, "classname")) {
                    current_entity.class_name = value_buffer;
                    current_line_no += 1;
                    continue;
                }

                if(current_entity.kv.contains(key_buffer)) {
                    // This is not a death sentence, it's just that
                    // the most recent value std::getline-wise will be stored
                    spdlog::warn("mapfile: {}:{}: duplicate key '{}'", path.string(), current_line_no, key_buffer);
                }

                current_entity.kv.insert_or_assign(key_buffer, value_buffer);
                current_line_no += 1;
                continue;
            }

            spdlog::critical("mapfile: {}:{}: syntax error: {}", path.string(), current_line_no, line);
            std::terminate();
        }

        if(current_parse == PARSE_BRUSH) {
            static char texture_buffer[2048];
            static Mapfile_BrushPlane plane;
            static Eigen::Vector3f points[3];

            std::memset(texture_buffer, 0, sizeof(texture_buffer));

            auto plane_sscanf_result =
                std::sscanf(line.c_str(), "( %f %f %f ) ( %f %f %f ) ( %f %f %f ) %2047[^\r\n\t ] [ %f %f %f %f ] [ %f %f %f %f ] %f %f %f",
                    &points[0].x(), &points[0].y(), &points[0].z(), &points[1].x(), &points[1].y(), &points[1].z(), &points[2].x(),
                    &points[2].y(), &points[2].z(), texture_buffer, &plane.u_normal.x(), &plane.u_normal.y(), &plane.u_normal.z(),
                    &plane.uv_offset.x(), &plane.v_normal.x(), &plane.v_normal.y(), &plane.v_normal.z(), &plane.uv_offset.y(),
                    &plane.uv_rotation_angle, &plane.uv_scale.x(), &plane.uv_scale.y());

            if(plane_sscanf_result == 21) {
                plane.hyperplane = Eigen::Hyperplane<float, 3>::Through(points[0], points[1], points[2]);
                plane.texture = texture_buffer;
                current_brush.planes.push_back(plane);
                current_line_no += 1;
                continue;
            }

            spdlog::critical("mapfile: {}:{}: syntax error", path.string(), current_line_no, line);
            std::terminate();
        }

        spdlog::critical("mapfile: {}:{}: if you read this, something very wrong has happened", path.string(), current_line_no);
        std::terminate();
    }

    if(mapfile::worldspawn == nullptr) {
        spdlog::critical("mapfile: {}: no worldspawn", path.string());
        std::terminate();
    }
}
