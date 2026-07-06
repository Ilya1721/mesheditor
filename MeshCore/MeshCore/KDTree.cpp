#include "KDTree.h"

#include <algorithm>

namespace
{
  using namespace MeshCore;

  std::unique_ptr<KDNode> createKDTree(
    std::vector<Vertex>& vertices, int begin, int end, int depth
  )
  {
    if (begin >= end)
    {
      return nullptr;
    }

    auto axis = depth % 3;
    auto mid = (begin + end) / 2;
    std::nth_element(
      vertices.begin() + begin, vertices.begin() + mid, vertices.begin() + end,
      [axis](const Vertex& left, const Vertex& right)
      {
        if (axis == 0)
          return left.pos.x < right.pos.x;
        if (axis == 1)
          return left.pos.y < right.pos.y;
        return left.pos.z < right.pos.z;
      }
    );

    auto node = std::make_unique<KDNode>();
    node->vertex = vertices[mid];
    node->left = createKDTree(vertices, begin, mid, depth + 1);
    node->right = createKDTree(vertices, mid + 1, end, depth + 1);

    return node;
  }

  void findNearestNode(
    KDNode* node,
    const Vertex& vertex,
    int depth,
    const std::unordered_set<KDNode*>& nearestNodes,
    KDNode*& nearestNode,
    float& shortestDist
  )
  {
    if (!node)
    {
      return;
    }

    auto dist = glm::distance(node->vertex.pos, vertex.pos);
    if (dist < shortestDist && nearestNodes.find(node) == nearestNodes.end())
    {
      nearestNode = node;
      shortestDist = dist;
    }

    auto axis = depth % 3;
    float diff = 0;
    if (axis == 0)
      diff = vertex.pos.x - node->vertex.pos.x;
    else if (axis == 1)
      diff = vertex.pos.y - node->vertex.pos.y;
    else
      diff = vertex.pos.z - node->vertex.pos.z;

    const auto& nearChild = diff < 0 ? node->left : node->right;
    findNearestNode(
      nearChild.get(), vertex, depth + 1, nearestNodes, nearestNode, shortestDist
    );

    if (std::abs(diff) < shortestDist)
    {
      const auto& farChild = diff < 0 ? node->right : node->left;
      findNearestNode(
        farChild.get(), vertex, depth + 1, nearestNodes, nearestNode, shortestDist
      );
    }
  }
}

namespace MeshCore
{
  KDTree::KDTree(std::vector<Vertex>& vertices)
  {
    mRoot = createKDTree(vertices, 0, vertices.size(), 0);
  }

  std::vector<Vertex> KDTree::getNearestVertices(const Vertex& vertex, int k) const
  {
    std::vector<Vertex> vertices;
    std::unordered_set<KDNode*> nearestNodes;
    for (int idx = 0; idx < k; ++idx)
    {
      KDNode* nearestNode = nullptr;
      float shortestDist = std::numeric_limits<float>::max();
      findNearestNode(mRoot.get(), vertex, 0, nearestNodes, nearestNode, shortestDist);
      vertices.push_back(nearestNode->vertex);
      nearestNodes.insert(nearestNode);
    }

    return vertices;
  }
}
