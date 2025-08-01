#include "Intersection.h"

#include "GeometryCore/Numeric.h"
#include "Object3D.h"

namespace RenderSystem
{
  void RaySurfaceIntersection::setClosest(
    const MeshCore::MeshIntersectionData& meshIntersectionData,
    const Point3D& referencePoint
  )
  {
    if (surfaceIndices.empty() ||
        isCloser(meshIntersectionData.intersectionPoint, point, referencePoint))
    {
      surface = meshIntersectionData.intersectedSurface;
      surfaceIndices = meshIntersectionData.interestedFacesIndices;
      point = meshIntersectionData.intersectionPoint;
    }
  }
}  // namespace RenderSystem