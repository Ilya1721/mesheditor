#pragma once

#include "BRepCurve.h"

namespace MeshCore
{
  struct BRepFace;
  struct BRepVertex;

  struct BRepEdge
  {
    std::unique_ptr<Curve3D> curve;
  };

  struct BRepHalfEdge
  {
    std::unique_ptr<Curve2D> pCurve;
    BRepVertex* vertex = nullptr;
    BRepEdge* edge;
    BRepFace* face;
    BRepHalfEdge* prev;
    BRepHalfEdge* next;
    bool isReversed;
  };

  struct BRepLoop
  {
    BRepHalfEdge* start;
  };
}
