#include "Intersection.h"

#include "MeshCore/Intersection.h"

using namespace MeshCore;

namespace RenderSystem
{
  void RaySurfaceIntersection::assign(const MeshIntersection& meshIntersection) 
  {
    surface = meshIntersection.intersectedSurface;
    surfaceIndices = meshIntersection.intersectedFacesIndices;
    point = meshIntersection.intersectionPoint;
  }
}  // namespace RenderSystem