#include "Octree.h"

namespace
{
  using namespace MeshCore;

  int getChildIndex(const glm::vec3& point, const glm::vec3& center)
  {
    int childIndex = 0;
    for (int coordIdx = 0; coordIdx < 3; ++coordIdx)
    {
      if (point[coordIdx] > center[coordIdx])
      {
        childIndex |= (1 << coordIdx);
      }
    }

    return childIndex;
  }

  void subdivide(OctreeNode* node)
  {
    node->isLeaf = false;
    auto center = node->bounds.getCenter();
    for (int childIdx = 0; childIdx < OCTREE_SUBDIVISION; ++childIdx)
    {
      glm::vec3 newMin = node->bounds.getMin();
      glm::vec3 newMax = node->bounds.getMax();

      if (childIdx & 1)
        newMin.x = center.x;
      else
        newMax.x = center.x;
      if (childIdx & 2)
        newMin.y = center.y;
      else
        newMax.y = center.y;
      if (childIdx & 4)
        newMin.z = center.z;
      else
        newMax.z = center.z;

      auto child = std::make_unique<OctreeNode>();
      child->bounds = AABBox(newMin, newMax);
      child->depth = node->depth + 1;
      node->children[childIdx] = std::move(child);
    }
  }

  void insert(OctreeNode* node, const Vertex& vertex)
  {
    const auto& bounds = node->bounds;
    if (!bounds.isPointInside(vertex.pos))
    {
      return;
    }

    if (!node->isLeaf)
    {
      int childIdx = getChildIndex(vertex.pos, bounds.getCenter());
      insert(node->children[childIdx].get(), vertex);
      return;
    }

    node->vertices.push_back(vertex);
    if (node->vertices.size() > OCTREE_MAX_POINTS && node->depth < OCTREE_MAX_DEPTH)
    {
      subdivide(node);
      for (const auto& nodeVertex : node->vertices)
      {
        int childIdx = getChildIndex(nodeVertex.pos, bounds.getCenter());
        insert(node->children[childIdx].get(), nodeVertex);
      }
      node->vertices.clear();
    }
  }

  void query(OctreeNode* node, const AABBox& bbox, std::vector<Vertex>& result)
  {
    if (!node->bounds.intersects(bbox))
    {
      return;
    }

    if (node->isLeaf)
    {
      for (const auto& vertex : node->vertices)
      {
        if (bbox.isPointInside(vertex.pos))
        {
          result.push_back(vertex);
        }
      }
      return;
    }

    for (const auto& child : node->children)
    {
      if (child)
      {
        query(child.get(), bbox, result);
      }
    }
  }

  void collectNodes(OctreeNode* root, std::vector<OctreeNode*>& nodes, bool leafOnly)
  {
    if (!root)
    {
      return;
    }

    if (!leafOnly || root->isLeaf)
    {
      nodes.push_back(root);
    }

    for (const auto& child : root->children)
    {
      collectNodes(child.get(), nodes, leafOnly);
    }
  }
}

namespace MeshCore
{
  Octree::Octree(const std::vector<Vertex>& vertices)
  {
    mRoot = std::make_unique<OctreeNode>();
    mRoot->bounds.applyVertices(vertices);
    for (const auto& vertex : vertices)
    {
      insert(mRoot.get(), vertex);
    }
  }

  void Octree::getVerticesInsideSector(const AABBox& bbox, std::vector<Vertex>& result)
    const
  {
    query(mRoot.get(), bbox, result);
  }
}
