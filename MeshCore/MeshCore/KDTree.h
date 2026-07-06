#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Vertex.h"

namespace MeshCore
{
  struct KDNode
  {
    Vertex vertex;
    std::unique_ptr<KDNode> left;
    std::unique_ptr<KDNode> right;
  };

  class KDTree
  {
   public:
    KDTree(std::vector<Vertex>& vertices);

    std::vector<Vertex> getNearestVertices(const Vertex& vertex, int k) const;

   private:
    std::unique_ptr<KDNode> mRoot;
  };
}
