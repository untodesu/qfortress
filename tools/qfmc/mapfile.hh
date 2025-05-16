#ifndef QFMC_MAPFILE_HH
#define QFMC_MAPFILE_HH
#pragma once

struct MAP_BrushPlane final {
    glm::fvec3 points[3];
    std::string texture;
    glm::fvec2 uv_scale;     ///< Texture scale
    glm::fvec4 u_projection; ///< XYZ - normal, W - offset
    glm::fvec4 v_projection; ///< XYZ - normal, W - offset
    float rotation;          ///< Texture rotation
};

using MAP_Brush = std::vector<MAP_BrushPlane>;

struct MAP_Entity final {
    std::string class_name;
    std::unordered_map<std::string, std::string> kv;
    std::vector<MAP_Brush> geometry;
};

namespace mapfile
{
extern std::vector<MAP_Entity> entities;
extern const MAP_Entity* worldspawn;
} // namespace mapfile

namespace mapfile
{
void load(const std::filesystem::path& path);
} // namespace mapfile

#endif // QFMC_MAPFILE_HH
