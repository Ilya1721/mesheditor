#include "Intersection.h"

#include "GeometryCore/Numeric.h"

namespace MeshCore
{
    void RaySurfaceIntersection::setClosest(const RaySurfaceIntersection& other, const Point3D& referencePoint)
    {
        if (surfaceIndices.empty() || isCloser(other.point, point, referencePoint))
        {
            surface = other.surface;
            surfaceIndices = other.surfaceIndices;
            point = other.point;
        }
    }
}