#ifndef QFMC_GEOMETRY_HH
#define QFMC_GEOMETRY_HH
#pragma once

struct Mapfile_Entity;

struct Geometry_Vertex final {
    Vector3f position;
    Vector2f texcoord;
};

struct Geometry_Plane final {
    std::vector<Geometry_Vertex> vertices;
    std::string texture_name;
    Vector3f normal;
};

struct Geometry_Brush final {
    std::vector<Geometry_Plane> planes;
};

struct Geometry_Entity final {
    std::vector<Geometry_Brush> brushes;
    const Mapfile_Entity* mf_entity;
};

namespace geometry
{
extern std::vector<Geometry_Entity> entities;
extern const Geometry_Entity* worldspawn;
} // namespace geometry

namespace geometry
{
void build(void);
} // namespace geometry

#endif // QFMC_GEOMETRY_HH
