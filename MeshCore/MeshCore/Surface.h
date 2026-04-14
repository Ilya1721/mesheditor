#pragma once

#include <glm/glm.hpp>
#include <unordered_set>

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
    glm::vec3 normal {};

   private:
    Face* mInitialFace = nullptr;
  };
}  // namespace MeshCore
