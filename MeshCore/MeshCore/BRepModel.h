#pragma once

#include <memory>
#include <vector>

#include "BRepEdge.h"
#include "BRepFace.h"
#include "BRepSurface.h"
#include "BRepVertex.h"

namespace MeshCore
{
  struct BRepModel
  {
    std::vector<std::unique_ptr<BRepFace>> faces;
    std::vector<std::unique_ptr<BRepEdge>> edges;
    std::vector<std::unique_ptr<BRepHalfEdge>> halfEdges;
    std::vector<std::unique_ptr<BRepVertex>> vertices;
    std::vector<std::unique_ptr<BRepSurface>> surfaces;
  };
}
