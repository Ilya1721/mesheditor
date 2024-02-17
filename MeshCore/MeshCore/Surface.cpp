#include "Surface.h"

#include "Face.h"
#include "Mesh.h"

namespace MeshCore
{
    Surface::Surface(Face* initialFace, bool collectAdjacentFaces) :
        normal(initialFace->calcNormal())
    {
        if (collectAdjacentFaces)
        {
            faces = initialFace->getAdjacentFaces(true, &normal);
        }

        faces.insert(initialFace);
    }
}