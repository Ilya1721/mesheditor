#pragma once

#include <vector>

namespace MeshCore
{
  class BRepSurface;
  class BRepLoop;

  struct BRepFace
  {
    BRepSurface* surface;
    BRepLoop* outerLoop;
    std::vector<BRepLoop*> innerLoops;
  };
}
