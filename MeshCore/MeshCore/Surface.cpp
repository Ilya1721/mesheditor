#include "Surface.h"

#include "Face.h"
#include "GeometryCore/Plane.h"
#include "Mesh.h"

namespace MeshCore
{
  Surface::Surface(Face* initialFace)
    : mInitialFace(initialFace), normal(initialFace->calcNormal())
  {
    faces = initialFace->getAdjacentFaces(true, &normal);
    faces.insert(initialFace);
  }

  GeometryCore::Plane Surface::getPerpendicularPlane() const
  {
    auto facePoints = mInitialFace->getVerticesPositions();
    auto planeNormal = glm::normalize(glm::cross(normal, facePoints[1] - facePoints[0]));

    return {facePoints[0], planeNormal};
  }
}  // namespace MeshCore