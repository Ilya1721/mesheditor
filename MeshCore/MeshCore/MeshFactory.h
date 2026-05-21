#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "BRepCurve.h"
#include "GeometryCore/Line.h"
#include "GeometryCore/Plane.h"
#include "GeometryCore/Ray.h"
#include "Vertex.h"

using namespace GeometryCore;

namespace MeshCore
{
  std::vector<Vertex> createSphere(float radius);
  std::vector<Vertex> createCube(float sideLength);
  std::vector<Vertex> createCircle(float radius, const glm::vec3& normal);
  std::vector<Vertex> createPlane(const Plane& plane, float width, float length);
  std::vector<Vertex> createLine(const GeometryCore::Line& line, bool withArrowHead);
  std::vector<Vertex> createRay(const GeometryCore::Ray& ray, float length);
  std::vector<Vertex> getBRepCurveVertices(const NURBSCurve3D& curve, size_t segments);
}  // namespace MeshCore
