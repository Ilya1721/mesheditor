#pragma once

#include "Mesh.h"

namespace MeshCore
{
  using PointCloud = std::vector<Vertex>;

  PointCloud mergePointClouds(const std::vector<PointCloud>& pointClouds);
  std::unique_ptr<Mesh> pointCloudToMesh(const PointCloud& pointCloud);
}
