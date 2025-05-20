#include "qfmc/pch.hh"

#include "qfmc/geometry.hh"

#include "qfmc/mapfile.hh"

constexpr static float GEOMETRY_EPS = 1.0e-4f;

std::vector<Geometry_Entity> geometry::entities;
const Geometry_Entity* geometry::worldspawn = nullptr;

static Vector3f intersectPlanes(const Hyperplane3f& h1, const Hyperplane3f& h2, const Hyperplane3f& h3)
{
    const Vector3f n1(h1.normal());
    const Vector3f n2(h2.normal());
    const Vector3f n3(h3.normal());

    const auto d1 = -h1.offset();
    const auto d2 = -h2.offset();
    const auto d3 = -h3.offset();

    auto denominator = n1.dot(n2.cross(n3));

    if(std::abs(denominator) < GEOMETRY_EPS) {
        // Assume there is no intersection
        return Vector3f::Constant(std::numeric_limits<float>::quiet_NaN());
    }

    return Vector3f((d1 * (n2.cross(n3)) + d2 * (n3.cross(n1)) + d3 * (n1.cross(n2))) / denominator);
}

static void removeDuplicatePoints(std::vector<Eigen::Vector3f>& inout_points)
{
    auto last = inout_points.end();

    for(auto itr = inout_points.begin(); itr != last; ++itr) {
        last = std::remove_if(itr + 1, last, [&itr](const Vector3f& vector) {
            return itr->isApprox(vector, GEOMETRY_EPS);
        });
    }

    inout_points.erase(last, inout_points.end());
}

static std::vector<Vector3f> buildCounterclockwisePolygon(const std::vector<Vector3f>& points)
{
    std::vector<Vector3f> unique_points(points);

    removeDuplicatePoints(unique_points);

    if(unique_points.size() < 3) {
        // Degenerate polygon
        return {};
    }

    Vector3f centroid(Vector3f::Zero());
    for(const auto& point : unique_points)
        centroid += point;
    centroid /= static_cast<float>(unique_points.size());

    std::sort(unique_points.begin(), unique_points.end(), [&centroid](const Vector3f& u, const Vector3f& v) -> bool {
        Vector3f du(u - centroid);
        Vector3f dv(v - centroid);

        auto angle_u = std::atan2(du.y(), du.x());
        auto angle_v = std::atan2(dv.y(), dv.x());

        return angle_u < angle_v;
    });

    return unique_points;
}

static Geometry_Brush buildGeometryBrush(const std::vector<Mapfile_Plane>& mf_planes)
{
    Geometry_Brush result;

    for(std::size_t i = 0; i < mf_planes.size(); ++i) {
        Geometry_Plane plane;
        plane.normal = mf_planes[i].hyperplane.normal();
        plane.texture_name = mf_planes[i].texture_name;

        std::vector<Vector3f> points;

        for(std::size_t j = 0; j < mf_planes.size(); ++j) {
            for(std::size_t k = 0; k < mf_planes.size(); ++k) {
                if(i == j || i == k || j == k) {
                    // Planes must be unique
                    continue;
                }

                auto& hyperplane_i = mf_planes[i].hyperplane;
                auto& hyperplane_j = mf_planes[j].hyperplane;
                auto& hyperplane_k = mf_planes[k].hyperplane;
                auto intersection = intersectPlanes(hyperplane_i, hyperplane_j, hyperplane_k);

                if(intersection.allFinite()) {
                    points.push_back(intersection);
                }
            }
        }

        for(const auto& point : buildCounterclockwisePolygon(points)) {
            Geometry_Vertex vertex;
            vertex.position = point;
            vertex.texcoord.x() = 0.0f;
            vertex.texcoord.y() = 0.0f;
            plane.vertices.push_back(std::move(vertex));
        }

        if(!plane.vertices.empty()) {
            result.planes.push_back(std::move(plane));
        }
    }

    return result;
}

void geometry::build(void)
{
    for(const auto& mf_entity : mapfile::entities) {
        Geometry_Entity entity;
        entity.mf_entity = &mf_entity;

        for(auto& mf_brush : mf_entity.brushes) {
            entity.brushes.push_back(buildGeometryBrush(mf_brush.planes));
        }

        geometry::entities.push_back(std::move(entity));

        if(mapfile::worldspawn == &mf_entity) {
            geometry::worldspawn = &geometry::entities.back();
        }
    }
}
