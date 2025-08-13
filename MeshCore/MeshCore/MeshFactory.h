#pragma once

#include <memory>

#include "Mesh.h"

namespace MeshCore
{
  std::unique_ptr<Mesh> createSphere(float radius);
}  // namespace MeshCore
