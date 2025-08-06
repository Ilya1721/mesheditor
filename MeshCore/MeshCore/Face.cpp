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

  std::optional<Point3D> Face::getIntersectionPoint(const Ray& ray) const
  {
    Plane facePlane(halfEdge->vertex->pos, calcNormal());
    auto rayPlaneIntersectionPoint = facePlane.getIntersectionPoint(ray);

    if (rayPlaneIntersectionPoint && isPointInside(rayPlaneIntersectionPoint.value()))
    {
      return rayPlaneIntersectionPoint;
    }

    return {};
  }

  Vector3D Face::calcNormal() const
  {
    auto firstEdge = halfEdge->next->vertex->pos - halfEdge->vertex->pos;
    auto secondEdge = halfEdge->prev->vertex->pos - halfEdge->next->vertex->pos;

    return glm::cross(firstEdge, secondEdge);
  }

  bool Face::isPointInside(const Point3D& point) const
  {
    auto verticesPositions = getVerticesPositions();
    std::array<std::array<Vector3D, 3>, 3> vertexPointEdges {};
    vertexPointEdges[0] = {
      point - verticesPositions[1], point - verticesPositions[0],
      verticesPositions[1] - verticesPositions[0]
    };
    vertexPointEdges[1] = {
      point - verticesPositions[2], point - verticesPositions[0],
      verticesPositions[2] - verticesPositions[0]
    };
    vertexPointEdges[2] = {
      point - verticesPositions[1], point - verticesPositions[2],
      verticesPositions[2] - verticesPositions[1]
    };

    std::array<std::array<float, 3>, 3> vertexPointEdgesLengths {};
    for (int triangleIdx = 0; triangleIdx < 3; ++triangleIdx)
    {
      for (int edgeIdx = 0; edgeIdx < 3; ++edgeIdx)
      {
        vertexPointEdgesLengths[triangleIdx][edgeIdx] =
          glm::length(vertexPointEdges[triangleIdx][edgeIdx]);
      }
    }

    float trianglesSquaresSum = 0.0f;

    for (int triangleIdx = 0; triangleIdx < 3; ++triangleIdx)
    {
      auto crossProduct =
        glm::cross(vertexPointEdges[triangleIdx][1], vertexPointEdges[triangleIdx][2]);
      if (!isEqual(crossProduct, Vector3D(0.0f, 0.0f, 0.0f)))
      {
        trianglesSquaresSum += getSquareOfTriangle(vertexPointEdgesLengths[triangleIdx]);
      }
    }

    return glm::epsilonEqual(
      std::roundf(trianglesSquaresSum), std::roundf(getSquare()), 1e-6f
    );
  }

  std::vector<Vector3D> Face::getAllEdges() const
  {
    std::vector<Vector3D> edges;
    EdgeWalker edgeWalker(halfEdge);
    edgeWalker.forEach(
      [&edges](const HalfEdge* edge)
      { edges.emplace_back(edge->next->vertex->pos - edge->vertex->pos); }
    );

    return edges;
  }

  std::vector<HalfEdge*> Face::getAllHalfEdges() const
  {
    std::vector<HalfEdge*> edges;
    EdgeWalker edgeWalker(halfEdge);
    edgeWalker.forEach([&edges](HalfEdge* edge) { edges.push_back(edge); });

    return edges;
  }

  std::unordered_set<Face*> Face::getAdjacentFaces(
    bool filterByNormal, const Vector3D* normalPtr
  ) const
  {
    std::unordered_set<Face*> uniqueAdjacentFaces;

    for (const auto& edge : getAllHalfEdges())
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
      else { ++adjacentFaceIt; }
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

  float Face::getSquare() const
  {
    auto edges = getAllEdges();
    return getSquareOfTriangle(
      {glm::length(edges[0]), glm::length(edges[1]), glm::length(edges[2])}
    );
  }

  void Face::move(
    const Vector3D& movement, std::unordered_set<UniqueVertex*>& alreadyChangedVertices
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

  std::vector<Point3D> Face::getVerticesPositions() const
  {
    std::vector<Point3D> verticesPositions;
    EdgeWalker edgeWalker(halfEdge);
    edgeWalker.forEach([&verticesPositions](const HalfEdge* edge)
                       { verticesPositions.emplace_back(edge->vertex->pos); });

    return verticesPositions;
  }
}  // namespace MeshCore