#pragma once

#include "Face.h"

#include <glm/gtx/vector_query.hpp>

#include "HalfEdge.h"
#include "Vertex.h"
#include "OutgoingEdgeFinder.h"

using namespace GeometryCore;

namespace MeshCore
{
    std::optional<glm::vec3> Face::getIntersectionPoint(const GeometryCore::Ray& ray) const
    {
        auto faceNormal = calcNormal();
        GeometryCore::Plane facePlane(halfEdge->vertex->pos, faceNormal);

        auto rayPlaneIntersectionPoint = ray.findIntersection(facePlane);
        if (rayPlaneIntersectionPoint.has_value() && isPointInside(rayPlaneIntersectionPoint.value(), faceNormal))
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

    bool Face::isPointInside(const glm::vec3& point, const glm::vec3& faceNormal) const
    {
        auto faceEdges = getAllGeometryEdges();
        auto pointToVerticesVectors = getAllVerticesToPointVectors(point);
        std::vector<bool> directionSigns;

        for (int edgeIdx = 0; edgeIdx < 3; ++edgeIdx)
        {
            auto crossProduct = glm::cross(faceEdges[edgeIdx], pointToVerticesVectors[edgeIdx]);
            auto sign = glm::dot(crossProduct, faceNormal) > 0.0f;
            if (directionSigns.size() > 0 && directionSigns.back() != sign)
            {
                return false;
            }
            directionSigns.push_back(sign);
        }

        return true;
    }

    std::vector<glm::vec3> Face::getAllGeometryEdges() const
    {
        std::vector<glm::vec3> edges;

        auto startHalfEdge = halfEdge;
        auto currentHalfEdge = startHalfEdge;
        do
        {
            edges.emplace_back(currentHalfEdge->next->vertex->pos - currentHalfEdge->vertex->pos);
            currentHalfEdge = currentHalfEdge->next;
        } while (currentHalfEdge != startHalfEdge);

        return edges;
    }

    std::vector<HalfEdge*> Face::getAllEdges() const
    {
        std::vector<HalfEdge*> edges;

        auto startHalfEdge = halfEdge;
        auto currentHalfEdge = startHalfEdge;
        do
        {
            edges.push_back(currentHalfEdge);
            currentHalfEdge = currentHalfEdge->next;
        } while (currentHalfEdge != startHalfEdge);

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
        auto startHalfEdge = halfEdge;
        auto currentHalfEdge = startHalfEdge;
        do
        {
            if (currentHalfEdge->vertex == vertex)
            {
                return currentHalfEdge;
            }
        } while (currentHalfEdge != startHalfEdge);

        return nullptr;
    }

    std::vector<glm::vec3> Face::getAllVerticesToPointVectors(const glm::vec3& point) const
    {
        std::vector<glm::vec3> verticesToPointVectors;

        auto startHalfEdge = halfEdge;
        auto currentHalfEdge = startHalfEdge;
        do
        {
            verticesToPointVectors.emplace_back(point - currentHalfEdge->vertex->pos);
            currentHalfEdge = currentHalfEdge->next;
        } while (currentHalfEdge != startHalfEdge);

        return verticesToPointVectors;
    }
}