#pragma once

namespace MeshCore
{
  struct UniqueVertex;
  struct Face;
  class Curve2D;

  struct HalfEdge
  {
    HalfEdge* prev = nullptr;
    HalfEdge* next = nullptr;
    HalfEdge* twin = nullptr;
    UniqueVertex* vertex = nullptr;
    Face* face = nullptr;
    Curve2D* curve = nullptr;
  };
}  // namespace MeshCore
