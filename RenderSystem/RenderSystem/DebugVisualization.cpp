#include "DebugVisualization.h"

#include <fstream>

namespace RenderSystem
{
  void writeVerticesToObjFile(const std::vector<Vertex>& vertices, const path& filePath)
  {
    std::ofstream file(filePath, std::ios::out | std::ios::trunc);

    if (!file.is_open())
    {
      throw std::runtime_error("Failed to open file: " + filePath.string());
    }

    for (const auto& vertex : vertices)
    {
      file << "v " << vertex.pos.x << ' ' << vertex.pos.y << ' ' << vertex.pos.z << '\n';
    }

    file.close();
  }

  void writeMeshToObjFile(const std::vector<Vertex>& vertices, const path& filePath)
  {
    std::ofstream file(filePath, std::ios::out | std::ios::trunc);

    if (!file.is_open())
    {
      throw std::runtime_error("Failed to open file: " + filePath.string());
    }

    for (const auto& vertex : vertices)
    {
      file << "v " << vertex.pos.x << " " << vertex.pos.y << " " << vertex.pos.z << "\n";
    }

    // Write vertex normals
    for (const auto& vertex : vertices)
    {
      file << "vn " << vertex.normal.x << " " << vertex.normal.y << " " << vertex.normal.z
           << "\n";
    }

    for (size_t i = 1; i < vertices.size(); i += 3)
    {
      file << "f ";
      file << i << "//" << i << " ";
      file << i + 1 << "//" << i + 1 << " ";
      file << i + 2 << "//" << i + 2 << "\n";
    }
  }
}  // namespace RenderSystem
