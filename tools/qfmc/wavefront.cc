#include "qfmc/pch.hh"

#include "qfmc/wavefront.hh"

#include "qfmc/geometry.hh"
#include "qfmc/mapfile.hh"

void wavefront::save(const std::filesystem::path& path)
{
    std::ofstream file(path, std::ios::out);

    if(!file.is_open()) {
        spdlog::critical("geometry: {}: cannot open", path.string());
        std::terminate();
    }

    std::size_t vdx = 0;

    for(std::size_t i = 0; i < geometry::entities.size(); ++i) {
        const auto& entity = geometry::entities.at(i);

        file << "# entity " << i << " [" << entity.mf_entity->classname << "]" << std::endl;

        for(std::size_t j = 0; j < entity.brushes.size(); ++j) {
            const auto& brush = entity.brushes.at(j);

            for(std::size_t k = 0; k < brush.planes.size(); ++k) {
                const auto& plane = brush.planes.at(k);

                // file << std::endl << "g entity_" << i << "_brush_" << j << "_plane_" << k << std::endl << std::endl;

                for(const auto& vertex : plane.vertices) {
                    file << "v " << vertex.position.x() << " " << vertex.position.y() << " " << vertex.position.z() << std::endl;
                }

                for(std::size_t i = 2; i < plane.vertices.size(); ++i) {
                    file << std::format("f {} {} {}", vdx + 1, vdx + i, vdx + i + 1) << std::endl;
                }

                vdx += plane.vertices.size();
            }
        }
    }

    file.close();
}
