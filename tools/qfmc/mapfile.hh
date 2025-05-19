#ifndef QFMC_MAPFILE_HH
#define QFMC_MAPFILE_HH
#pragma once

struct Mapfile_BrushPlane final {
    Eigen::Hyperplane<float, 3> hyperplane;
    Eigen::Vector3f u_normal, v_normal;
    Eigen::Vector2f uv_offset, uv_scale;
    float uv_rotation_angle;
    std::string texture;
};

struct Mapfile_Brush final {
    std::vector<Mapfile_BrushPlane> planes;
};

struct Mapfile_Entity final {
    std::unordered_map<std::string, std::string> kv;
    std::vector<Mapfile_Brush> geometry;
    std::string class_name;
};

namespace mapfile
{
extern std::vector<Mapfile_Entity> entities;
extern const Mapfile_Entity* worldspawn;
} // namespace mapfile

namespace mapfile
{
void load(const std::filesystem::path& path);
} // namespace mapfile

#endif // QFMC_MAPFILE_HH
