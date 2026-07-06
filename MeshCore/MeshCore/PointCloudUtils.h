#pragma once

#include "Mesh.h"

namespace MeshCore
{
  std::unique_ptr<Mesh> pointCloudToMesh(const std::vector<Vertex>& pointCloud);
}
