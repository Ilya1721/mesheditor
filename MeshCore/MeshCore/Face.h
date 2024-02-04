#pragma once

#include <vector>

#include "GeometryCore/Ray.h"

namespace MeshCore
{
    struct HalfEdge;

    struct Face
    {
        HalfEdge* halfEdge = nullptr;

        std::optional<glm::vec3> getIntersectionPoint(const GeometryCore::Ray& ray) const;
        glm::vec3 calcNormal() const;
        bool isPointInside(const glm::vec3& point, const glm::vec3& faceNormal) const;
        std::vector<glm::vec3> getAllEdges() const;

    private:
        std::vector<glm::vec3> getAllVerticesToPointVectors(const glm::vec3& point) const;
    };
}
