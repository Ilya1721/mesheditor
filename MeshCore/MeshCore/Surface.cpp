#include "Surface.h"

#include "GeometryCore/Plane.h"

#include "Face.h"
#include "Mesh.h"
#include "Object3D.h"

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
        auto planeNormal = glm::normalize(glm::cross(normal, facePoints[1] - facePoints[0]));

        return { facePoints[0], planeNormal };
    }

    Object3D* Surface::getParentObject() const
    {
        if (!mInitialFace) {
            return nullptr;
        }

        return mInitialFace->parentMesh->getParentObject();
    }
}