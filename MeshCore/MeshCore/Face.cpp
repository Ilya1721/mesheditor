#pragma once

#include "Face.h"

#include <algorithm>
#include <array>

#include <glm/gtx/vector_query.hpp>

#include "GeometryCore/Numeric.h"

#include "HalfEdge.h"
#include "Vertex.h"
#include "OutgoingEdgeFinder.h"
#include "EdgeWalker.h"

using namespace GeometryCore;

namespace MeshCore
{
    std::optional<glm::vec3> Face::getIntersectionPoint(const GeometryCore::Ray& ray) const
    {
        auto faceNormal = calcNormal();
        GeometryCore::Plane facePlane(halfEdge->vertex->pos, faceNormal);

        auto rayPlaneIntersectionPoint = ray.findIntersection(facePlane);
        if (rayPlaneIntersectionPoint.has_value() && isPointInside(rayPlaneIntersectionPoint.value()))
        {
            return rayPlaneIntersectionPoint;
        }

        return {};
    }

    glm::vec3 Face::calcNormal() const
    {
        auto firstEdge = halfEdge->next->vertex->pos - halfEdge->vertex->pos;
        auto secondEdge = halfEdge->prev->vertex->pos - halfEdge->next->vertex->pos;

        return glm::cross(firstEdge, secondEdge);
    }

    bool Face::isPointInside(const glm::vec3& point) const
    {
        auto verticesPositions = getVerticesPositions();
        std::array<std::array<glm::vec3, 3>, 3> vertexPointEdges{};
        vertexPointEdges[0] = { point - verticesPositions[1], point - verticesPositions[0], verticesPositions[1] - verticesPositions[0] };
        vertexPointEdges[1] = { point - verticesPositions[2], point - verticesPositions[0], verticesPositions[2] - verticesPositions[0] };
        vertexPointEdges[2] = { point - verticesPositions[1], point - verticesPositions[2], verticesPositions[2] - verticesPositions[1] };

        std::array<std::array<float, 3>, 3> vertexPointEdgesLengths{};
        for (int triangleIdx = 0; triangleIdx < 3; ++triangleIdx)
        {
            for (int edgeIdx = 0; edgeIdx < 3; ++edgeIdx)
            {
                vertexPointEdgesLengths[triangleIdx][edgeIdx] = glm::length(vertexPointEdges[triangleIdx][edgeIdx]);
            }
        }

        float trianglesSquaresSum = 0.0f;

        for (int triangleIdx = 0; triangleIdx < 3; ++triangleIdx)
        {
            auto crossProduct = glm::cross(vertexPointEdges[triangleIdx][1], vertexPointEdges[triangleIdx][2]);
            if (!glm::all(glm::epsilonEqual(crossProduct, glm::vec3(0.0f, 0.0f, 0.0f), 1e-6f)))
            {
                trianglesSquaresSum += getTriangleSquare(vertexPointEdgesLengths[triangleIdx]);
            }
        }

        return glm::epsilonEqual(std::roundf(trianglesSquaresSum), std::roundf(getSquare()), 1e-6f);
    }

    std::vector<glm::vec3> Face::getAllGeometryEdges() const
    {
        std::vector<glm::vec3> edges;
        EdgeWalker edgeWalker(halfEdge);
        edgeWalker.forEach([&edges](HalfEdge* edge)
        {
            edges.emplace_back(edge->next->vertex->pos - edge->vertex->pos);
        });

        return edges;
    }

    std::vector<HalfEdge*> Face::getAllEdges() const
    {
        std::vector<HalfEdge*> edges;
        EdgeWalker edgeWalker(halfEdge);
        edgeWalker.forEach([&edges](HalfEdge* edge)
        {
            edges.push_back(edge);
        });

        return edges;
    }

    std::unordered_set<Face*> Face::getAdjacentFaces(bool filterByNormal, const glm::vec3* normalPtr) const
    {
        auto surfaceNormal = normalPtr ? *normalPtr : calcNormal();
        std::unordered_set<Face*> uniqueAdjacentFaces;

        for (const auto& edge : getAllEdges())
        {
            OutgoingEdgeFinder finder(edge);
            finder.collectAll();
            auto& adjacentFaces = finder.getOutgoingEdgesFaces();
            uniqueAdjacentFaces.insert(adjacentFaces.begin(), adjacentFaces.end());
        }

        for (auto adjacentFaceIt = uniqueAdjacentFaces.begin(); adjacentFaceIt != uniqueAdjacentFaces.end();)
        {
            if (filterByNormal && !glm::areCollinear((*adjacentFaceIt)->calcNormal(), surfaceNormal, 1e-6f))
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

    HalfEdge* Face::findOutgoingEdge(const Vertex* vertex) const
    {
        EdgeWalker edgeWalker(halfEdge);
        HalfEdge* outgoingEdge = nullptr;
        edgeWalker.forEach([&outgoingEdge, &vertex](HalfEdge* halfEdge)
        {
            if (halfEdge->vertex == vertex)
            {
                outgoingEdge = halfEdge;
                return;
            }
        });

        return outgoingEdge;
    }

    float Face::getSquare() const
    {
        auto edges = getAllGeometryEdges();
        return getTriangleSquare({ glm::length(edges[0]), glm::length(edges[1]), glm::length(edges[2]) });
    }

    void Face::move(const glm::vec3& movement, std::unordered_set<Vertex*>& alreadyChangedVertices)
    {
        EdgeWalker edgeWalker(halfEdge);
        edgeWalker.forEach([&movement, &alreadyChangedVertices](HalfEdge* halfEdge)
        {
            auto& halfEdgeVertex = halfEdge->vertex;
            if (alreadyChangedVertices.find(halfEdgeVertex) == alreadyChangedVertices.end())
            {
                halfEdge->vertex->pos += movement;
                alreadyChangedVertices.insert(halfEdge->vertex);
            }
        });
    }

    std::vector<glm::vec3> Face::getVerticesPositions() const
    {
        std::vector<glm::vec3> verticesPositions;
        EdgeWalker edgeWalker(halfEdge);
        edgeWalker.forEach([&verticesPositions](HalfEdge* edge)
        {
            verticesPositions.emplace_back(edge->vertex->pos);
        });

        return verticesPositions;
    }
}