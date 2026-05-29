#pragma once

#include <memory>
#include <vector>

#include "BRepDomain.h"
#include "BRepEdge.h"
#include "BRepUVMapper.h"

namespace MeshCore
{
  class BRepSurface;

  struct BRepFace
  {
    BRepSurface* surface;
    std::unique_ptr<BRepLoop> outerLoop;
    std::vector<std::unique_ptr<BRepLoop>> innerLoops;
    std::unique_ptr<BRepFaceUVMapper> uvMapper;

    SurfaceDomain getDomain() const;
    bool isPointInside(const glm::vec2& uv) const;
  };
}
