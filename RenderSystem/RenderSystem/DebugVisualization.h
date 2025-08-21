#pragma once

#include <filesystem>
#include <vector>

#include "MeshCore/Mesh.h"

using namespace MeshCore;
using namespace std::filesystem;

namespace RenderSystem
{
  void writeVerticesToObjFile(const std::vector<Vertex>& vertices, const path& filePath);
  void writeMeshToObjFile(const std::vector<Vertex>& vertices, const path& filePath);
}
