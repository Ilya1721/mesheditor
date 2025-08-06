#pragma once

#include <glm/glm.hpp>
#include <unordered_set>

#include "GeometryCore/Typedefs.h"

namespace GeometryCore
{
  struct Plane;
}

namespace MeshCore
{
  using namespace GeometryCore;

  struct Face;

  struct Surface
  {
    Surface() = default;
    Surface(Face* initialFace);

    Plane getPerpendicularPlane() const;

    std::unordered_set<Face*> faces;
    Vector3D normal {};

   private:
    Face* mInitialFace = nullptr;
  };
}  // namespace MeshCore
