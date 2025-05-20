#ifndef QFMC_MAPFILE_HH
#define QFMC_MAPFILE_HH
#pragma once

/// Represents a brush plane found in all the entities that
/// posess level geometry on a specific map; the hyperplane is
/// constructed in-place from three points provided by the map file
struct Mapfile_Plane final {
    Hyperplane3f hyperplane;    ///< Produced from three points in the file
    Vector3f texture_u_normal;  ///< Valve220: axis normal for axial projection
    Vector3f texture_v_normal;  ///< Valve220: axis normal for axial projection
    Vector2f texture_uv_offset; ///< Texture coordinate offset
    Vector2f texture_uv_scale;  ///< Texture coordinate scale
    float texture_uv_rotation;  ///< Texture coordinate rotation
    std::string texture_name;   ///< Texture name
};

/// Represents a single brush found in all entities that
/// posess level geometry on a specific map; a single entity
/// can have multiple brushes. Brushes are continiously numbered in logs
struct Mapfile_Brush final {
    std::vector<Mapfile_Plane> planes;
};

struct Mapfile_Entity final {
    std::unordered_map<std::string, std::string> kv;
    std::vector<Mapfile_Brush> brushes;
    std::string classname;

public:
    /// Retreives a KV pair's value if possible
    /// @param key The key name
    /// @param default_value Fallback value
    /// @return KV pair's value or \c default_value if not found
    const char* findKV(const std::string_view key, const char* default_value = nullptr) const;
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
