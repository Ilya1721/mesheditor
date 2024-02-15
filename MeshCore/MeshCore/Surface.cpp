#include "Surface.h"

#include "Face.h"

namespace MeshCore
{
    Surface::Surface(Face* initialFace)
    {
        normal = initialFace->calcNormal();
        faces = initialFace->getAdjacentFaces(&normal, true);
        faces.insert(initialFace);
    }
}