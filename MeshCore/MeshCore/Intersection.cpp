#include "Intersection.h"

#include "GeometryCore/Numeric.h"

namespace MeshCore
{
    void RaySurfaceIntersection::setClosest(const RaySurfaceIntersection& other, const glm::vec3& referencePoint)
    {
        if (surfaceIndices.empty() || GeometryCore::isCloser(other.point, point, referencePoint))
        {
            intersectedSurface = other.intersectedSurface;
            surfaceIndices = other.surfaceIndices;
            point = other.point;
        }
    }
}