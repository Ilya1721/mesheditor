#pragma once

#include "Face.h"

#include <algorithm>
#include <array>
#include <glm/gtx/vector_query.hpp>

#include "EdgeWalker.h"
#include "GeometryCore/Numeric.h"
#include "GeometryCore/Plane.h"
#include "GeometryCore/Ray.h"
#include "HalfEdge.h"
#include "OutgoingEdgeFinder.h"
#include "Vertex.h"

using namespace GeometryCore;

namespace MeshCore
{
  Face::Face(Mesh* parentMesh, HalfEdge* halfEdge)
    : parentMesh(parentMesh), halfEdge(halfEdge)
  {
  }

  std::optional<glm::vec3> Face::getIntersectionPoint(const Ray& ray) const
  {
    Plane facePlane(halfEdge->vertex->pos, calcNormal());
    auto rayPlaneIntersectionPoint = facePlane.getIntersectionPoint(ray);

    if (rayPlaneIntersectionPoint && isPointInside(rayPlaneIntersectionPoint.value()))
    {
      return rayPlaneIntersectionPoint;
    }

    return {};
  }

  glm::vec3 Face::calcNormal() const
  {
    const auto& vertices = getVerticesPositions();
    auto leftEdge = vertices[1] - vertices[0];
    auto rightEdge = vertices[2] - vertices[0];

    return glm::normalize(glm::cross(leftEdge, rightEdge));
  }

  bool Face::isPointInside(const glm::vec3& point) const
  {
    const auto& vertices = getVerticesPositions();
    std::array<glm::vec3, 6> littleTrianglesEdges {};
    littleTrianglesEdges[0] = point - vertices[0];
    littleTrianglesEdges[1] = point - vertices[1];
    littleTrianglesEdges[2] = point - vertices[1];
    littleTrianglesEdges[3] = point - vertices[2];
    littleTrianglesEdges[4] = point - vertices[2];
    littleTrianglesEdges[5] = point - vertices[0];

    float littleTrianglesTotalArea = 0.0f;
    for (size_t edgeIdx = 0; edgeIdx < 6; edgeIdx += 2)
    {
      const auto& leftEdge = littleTrianglesEdges[edgeIdx];
      const auto& rightEdge = littleTrianglesEdges[edgeIdx + 1];
      littleTrianglesTotalArea += getTriangleArea(leftEdge, rightEdge);
    }

    auto leftEdge = vertices[1] - vertices[0];
    auto rightEdge = vertices[2] - vertices[0];
    auto bigTriangleArea = getTriangleArea(leftEdge, rightEdge);

    return glm::epsilonEqual(littleTrianglesTotalArea, bigTriangleArea, 1e-6f);
  }

  std::vector<HalfEdge*> Face::getHalfEdges() const
  {
    std::vector<HalfEdge*> edges;
    EdgeWalker edgeWalker(halfEdge);
    edgeWalker.forEach([&edges](HalfEdge* edge) { edges.push_back(edge); });

    return edges;
  }

  std::unordered_set<Face*> Face::getAdjacentFaces(
    bool filterByNormal, const glm::vec3* normalPtr
  ) const
  {
    std::unordered_set<Face*> uniqueAdjacentFaces;

    for (const auto& edge : getHalfEdges())
    {
      OutgoingEdgeFinder finder(edge);
      finder.collectAll();
      auto& adjacentFaces = finder.getOutgoingEdgesFaces();
      uniqueAdjacentFaces.insert(adjacentFaces.begin(), adjacentFaces.end());
    }

    auto surfaceNormal = normalPtr ? *normalPtr : calcNormal();

    for (auto adjacentFaceIt = uniqueAdjacentFaces.begin();
         adjacentFaceIt != uniqueAdjacentFaces.end();)
    {
      if (filterByNormal &&
          !glm::areCollinear((*adjacentFaceIt)->calcNormal(), surfaceNormal, 1e-6f))
      {
        adjacentFaceIt = uniqueAdjacentFaces.erase(adjacentFaceIt);
      }
      else
      {
        ++adjacentFaceIt;
      }
    }

    return uniqueAdjacentFaces;
  }

  HalfEdge* Face::findOutgoingEdge(const UniqueVertex* vertex) const
  {
    EdgeWalker edgeWalker(halfEdge);
    HalfEdge* outgoingEdge = nullptr;
    edgeWalker.forEach(
      [&outgoingEdge, &vertex](HalfEdge* halfEdge)
      {
        if (halfEdge->vertex == vertex)
        {
          outgoingEdge = halfEdge;
          return;
        }
      }
    );

    return outgoingEdge;
  }

  void Face::move(
    const glm::vec3& movement, std::unordered_set<UniqueVertex*>& alreadyChangedVertices
  )
  {
    EdgeWalker edgeWalker(halfEdge);
    edgeWalker.forEach(
      [&movement, &alreadyChangedVertices](HalfEdge* halfEdge)
      {
        auto& halfEdgeVertex = halfEdge->vertex;
        if (alreadyChangedVertices.find(halfEdgeVertex) == alreadyChangedVertices.end())
        {
          halfEdge->vertex->updatePos(halfEdge->vertex->pos + movement);
          alreadyChangedVertices.insert(halfEdge->vertex);
        }
      }
    );
  }

  std::vector<glm::vec3> Face::getVerticesPositions() const
  {
    std::vector<glm::vec3> verticesPositions;
    EdgeWalker edgeWalker(halfEdge);
    edgeWalker.forEach([&verticesPositions](const HalfEdge* edge)
                       { verticesPositions.emplace_back(edge->vertex->pos); });

    return verticesPositions;
  }
}  // namespace MeshCore