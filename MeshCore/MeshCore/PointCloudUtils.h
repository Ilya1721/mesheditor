#pragma once

#include "Mesh.h"

namespace MeshCore
{
  std::vector<Vertex> getReconstructedVertices(
    const std::vector<Vertex>& pointCloud, const glm::vec3& bboxCenter
  );
}
