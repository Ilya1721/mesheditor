#pragma once

namespace MeshCore
{
  struct HalfEdge;
  class Curve2D;
  class Curve3D;

  struct BRepEdge
  {
    Curve3D* curve3D;
    Curve2D* curveOnSurface;
    HalfEdge* halfEdge;
  };

  struct BRepLoop
  {
    BRepEdge* start;
  };
}
