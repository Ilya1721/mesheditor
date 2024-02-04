#include "Intersection.h"

#include "GeometryCore/Numeric.h"

namespace MeshCore
{
    void RayFaceIntersection::setClosest(const RayFaceIntersection& other, const glm::vec3& referencePoint)
    {
        if (!face || GeometryCore::isCloser(other.point, point, referencePoint))
        {
            face = other.face;
            point = other.point;
        }
    }
}