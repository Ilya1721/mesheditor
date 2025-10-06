#pragma once

#include <memory>
#include <vector>

#include "Vertex.h"

namespace MeshCore
{
  std::vector<Vertex> createSphere(float radius);
  std::vector<Vertex> createCube(float sideLength);
}  // namespace MeshCore
