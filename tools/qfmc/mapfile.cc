#include "qfmc/pch.hh"

#include "qfmc/mapfile.hh"

#include "core/strtools.hh"

constexpr static unsigned int PARSE_NOTHING = 0U;
constexpr static unsigned int PARSE_ENTITY = 1U;
constexpr static unsigned int PARSE_BRUSH = 2U;

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
static void addMapEntity_worldspawn(const Mapfile_Entity& entity)
{
    if(mapfile::worldspawn) {
        spdlog::critical("mapfile: a level cannot have more than one worldspawn!");
        std::terminate();
    }

    auto mapversion = std::atoi(entity.findKV("mapversion", "0"));

    if(mapversion != VALVE220_MAPVERSION) {
        spdlog::critical("mapfile: unsupported mapversion [parsed {}, supporting {}]", mapversion, VALVE220_MAPVERSION);
        std::terminate();
    }

    mapfile::entities.push_back(entity);
    mapfile::worldspawn = &mapfile::entities.back();
}

static void addMapEntity(const Mapfile_Entity& entity)
{
    if(entity.classname == "worldspawn") {
        addMapEntity_worldspawn(entity);
        return;
    }

    mapfile::entities.push_back(entity);
}

const char* Mapfile_Entity::findKV(const std::string_view key, const char* default_value) const
{
    auto found_pair = kv.find(std::string(key));
    if(found_pair == kv.cend())
        return default_value;
    return found_pair->second.c_str();
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
    auto current_parse = PARSE_NOTHING;
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
            if(current_parse == PARSE_NOTHING) {
                current_parse = PARSE_ENTITY;
                current_entity.classname.clear();
                current_entity.brushes.clear();
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
                current_entity.brushes.push_back(current_brush);
                current_line_no += 1;
                continue;
            }

            if(current_parse == PARSE_ENTITY) {
                addMapEntity(current_entity);
                current_parse = PARSE_NOTHING;
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
                    current_entity.classname = value_buffer;
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
            static char texture_name_buffer[2048];
            static Mapfile_Plane plane;
            static Vector3f points[3];

            std::memset(texture_name_buffer, 0, sizeof(texture_name_buffer));

            auto plane_sscanf_result = std::sscanf(line.c_str(),
                "( %f %f %f ) ( %f %f %f ) ( %f %f %f ) %2047[^\r\n\t ] [ %f %f %f %f ] [ %f %f %f %f ] %f %f %f", &points[0].x(),
                &points[0].y(), &points[0].z(), &points[1].x(), &points[1].y(), &points[1].z(), &points[2].x(), &points[2].y(),
                &points[2].z(), texture_name_buffer, &plane.texture_u_normal.x(), &plane.texture_u_normal.y(), &plane.texture_u_normal.z(),
                &plane.texture_uv_offset.x(), &plane.texture_v_normal.x(), &plane.texture_v_normal.y(), &plane.texture_v_normal.z(),
                &plane.texture_uv_offset.y(), &plane.texture_uv_rotation, &plane.texture_uv_scale.x(), &plane.texture_uv_scale.y());

            if(plane_sscanf_result == 21) {
                plane.hyperplane = Hyperplane3f::Through(points[0], points[1], points[2]);
                plane.texture_name = texture_name_buffer;
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
