#include "Surface.h"

#include "GeometryCore/Plane.h"

#include "Face.h"
#include "Mesh.h"

namespace MeshCore
{
    Surface::Surface(Face* initialFace, bool collectAdjacentFaces) :
        mInitialFace(initialFace), normal(initialFace->calcNormal())
    {
        if (collectAdjacentFaces)
        {
            faces = initialFace->getAdjacentFaces(true, &normal);
        }

        faces.insert(initialFace);
    }

    GeometryCore::Plane Surface::getPerpendicularPlane() const
    {
        auto facePoints = mInitialFace->getVerticesPositions();
        auto planeNormal = glm::normalize(glm::cross(facePoints[1] - facePoints[0], normal));

        return { facePoints[0], planeNormal };
    }
}