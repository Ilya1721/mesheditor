#pragma once

#include <memory>

#include "AABBox.h"
#include "Constants.h"
#include "Vertex.h"

namespace MeshCore
{
  struct OctreeNode
  {
    AABBox bounds;
    std::vector<Vertex> vertices;
    std::unique_ptr<OctreeNode> children[OCTREE_SUBDIVISION];
    bool isLeaf = true;
    int depth = 0;
  };

  class Octree
  {
   public:
    Octree(const std::vector<Vertex>& vertices);

    void getVerticesInsideSector(const AABBox& bbox, std::vector<Vertex>& result) const;

   protected:
    std::unique_ptr<OctreeNode> mRoot;
  };
}
