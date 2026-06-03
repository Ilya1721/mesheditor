#pragma once

#include "Mesh.h"
#include "PointCloud.h"

namespace MeshCore
{
  PointCloud mergePointClouds(const std::vector<PointCloud>& pointClouds);
  std::unique_ptr<Mesh> pointCloudToMesh(const PointCloud& pointCloud);
}
