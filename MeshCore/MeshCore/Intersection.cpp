#include "Intersection.h"

#include "GeometryCore/Numeric.h"

namespace MeshCore
{
    void RayFaceIntersection::setClosest(const RayFaceIntersection& other, const glm::vec3& referencePoint)
    {
        if (faceIdx == -1 || GeometryCore::isCloser(other.point, point, referencePoint))
        {
            faceIdx = other.faceIdx;
            point = other.point;
        }
    }
}