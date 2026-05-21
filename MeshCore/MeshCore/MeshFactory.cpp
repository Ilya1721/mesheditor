#include "MeshFactory.h"

#include <cmath>
#include <numbers>
#include <vector>

#include "BRepCurve.h"
#include "Constants.h"
#include "GeometryCore/Numeric.h"

using namespace std::numbers;

namespace MeshCore
{
  std::vector<Vertex> createSphere(float radius)
  {
    std::vector<Vertex> tempVertices;
    constexpr size_t horizontalDivisions = 20;
    constexpr size_t verticalDivisions = 20;

    for (int i = 0; i <= horizontalDivisions; ++i)
    {
      float horizontalAngle = 2.0 * pi * i / horizontalDivisions;
      float sinHorizontal = std::sin(horizontalAngle);
      float cosHorizontal = std::cos(horizontalAngle);

      for (int j = 0; j <= verticalDivisions; ++j)
      {
        float verticalAngle = pi * j / verticalDivisions;
        float sinVertical = std::sin(verticalAngle);
        float cosVertical = std::cos(verticalAngle);

        float posX = radius * sinVertical * sinHorizontal;
        float posY = radius * cosVertical;
        float posZ = radius * sinVertical * cosHorizontal;

        Vertex vertex;
        vertex.pos = glm::vec3(posX, posY, posZ);
        vertex.normal = vertex.pos / radius;  // pos length is always radius
        tempVertices.push_back(vertex);
      }
    }

    std::vector<Vertex> vertices;
    for (size_t i = 0; i < tempVertices.size() - verticalDivisions - 2; ++i)
    {
      size_t nextIdx = i + 1;
      size_t nextDivIdxLower = (i + verticalDivisions + 2);
      size_t nextDivIdxHigher = (i + verticalDivisions + 1);

      if (i % (verticalDivisions + 1) == 0)
      {
        vertices.push_back(tempVertices[i]);
        vertices.push_back(tempVertices[nextIdx]);
        vertices.push_back(tempVertices[nextDivIdxLower]);
      }
      else if ((i + 2) % (verticalDivisions + 1) == 0)
      {
        vertices.push_back(tempVertices[i]);
        vertices.push_back(tempVertices[nextIdx]);
        vertices.push_back(tempVertices[nextDivIdxHigher]);
      }
      else if ((i + 1) % (verticalDivisions + 1) != 0)
      {
        vertices.push_back(tempVertices[i]);
        vertices.push_back(tempVertices[nextIdx]);
        vertices.push_back(tempVertices[nextDivIdxLower]);
        vertices.push_back(tempVertices[i]);
        vertices.push_back(tempVertices[nextDivIdxLower]);
        vertices.push_back(tempVertices[nextDivIdxHigher]);
      }
    }

    return vertices;
  }

  std::vector<Vertex> createCube(float sideLength)
  {
    float halfSide = sideLength * 0.5f;
    std::vector<Vertex> uniqueVertices;
    uniqueVertices.emplace_back(
      glm::vec3(-halfSide, -halfSide, -halfSide), glm::vec3(-1.0f, -1.0f, -1.0f),
      glm::vec2(0.0f, 0.0f)
    );
    uniqueVertices.emplace_back(
      glm::vec3(halfSide, -halfSide, -halfSide), glm::vec3(1.0f, -1.0f, -1.0f),
      glm::vec2(1.0f, 0.0f)
    );
    uniqueVertices.emplace_back(
      glm::vec3(halfSide, halfSide, -halfSide), glm::vec3(1.0f, 1.0f, -1.0f),
      glm::vec2(1.0f, 1.0f)
    );
    uniqueVertices.emplace_back(
      glm::vec3(-halfSide, halfSide, -halfSide), glm::vec3(-1.0f, 1.0f, -1.0f),
      glm::vec2(0.0f, 1.0f)
    );
    uniqueVertices.emplace_back(
      glm::vec3(-halfSide, -halfSide, halfSide), glm::vec3(-1.0f, -1.0f, 1.0f),
      glm::vec2(0.0f, 0.0f)
    );
    uniqueVertices.emplace_back(
      glm::vec3(halfSide, -halfSide, halfSide), glm::vec3(1.0f, -1.0f, 1.0f),
      glm::vec2(1.0f, 0.0f)
    );
    uniqueVertices.emplace_back(
      glm::vec3(halfSide, halfSide, halfSide), glm::vec3(1.0f, 1.0f, 1.0f),
      glm::vec2(1.0f, 1.0f)
    );
    uniqueVertices.emplace_back(
      glm::vec3(-halfSide, halfSide, halfSide), glm::vec3(-1.0f, 1.0f, 1.0f),
      glm::vec2(0.0f, 1.0f)
    );

    std::vector<Vertex> vertices;
    vertices.insert(
      vertices.end(), {uniqueVertices[0], uniqueVertices[1], uniqueVertices[2]}
    );
    vertices.insert(
      vertices.end(), {uniqueVertices[2], uniqueVertices[3], uniqueVertices[0]}
    );
    vertices.insert(
      vertices.end(), {uniqueVertices[4], uniqueVertices[5], uniqueVertices[6]}
    );
    vertices.insert(
      vertices.end(), {uniqueVertices[6], uniqueVertices[7], uniqueVertices[4]}
    );
    vertices.insert(
      vertices.end(), {uniqueVertices[4], uniqueVertices[0], uniqueVertices[3]}
    );
    vertices.insert(
      vertices.end(), {uniqueVertices[3], uniqueVertices[7], uniqueVertices[4]}
    );
    vertices.insert(
      vertices.end(), {uniqueVertices[1], uniqueVertices[5], uniqueVertices[6]}
    );
    vertices.insert(
      vertices.end(), {uniqueVertices[6], uniqueVertices[2], uniqueVertices[1]}
    );
    vertices.insert(
      vertices.end(), {uniqueVertices[4], uniqueVertices[5], uniqueVertices[1]}
    );
    vertices.insert(
      vertices.end(), {uniqueVertices[1], uniqueVertices[0], uniqueVertices[4]}
    );
    vertices.insert(
      vertices.end(), {uniqueVertices[3], uniqueVertices[2], uniqueVertices[6]}
    );
    vertices.insert(
      vertices.end(), {uniqueVertices[6], uniqueVertices[7], uniqueVertices[3]}
    );

    return vertices;
  }

  std::vector<Vertex> createCircle(float radius, const glm::vec3& normal)
  {
    return {};
  }

  std::vector<Vertex> createPlane(const Plane& plane, float width, float length)
  {
    std::vector<Vertex> vertices;
    auto halfWidth = width * 0.5f;
    auto halfLength = length * 0.5f;
    vertices.emplace_back(
      glm::vec3(halfWidth, halfLength, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)
    );
    vertices.emplace_back(
      glm::vec3(-halfWidth, halfLength, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)
    );
    vertices.emplace_back(
      glm::vec3(-halfWidth, -halfLength, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)
    );
    vertices.emplace_back(
      glm::vec3(-halfWidth, -halfLength, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)
    );
    vertices.emplace_back(
      glm::vec3(halfWidth, -halfLength, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)
    );
    vertices.emplace_back(
      glm::vec3(halfWidth, halfLength, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)
    );

    Plane defaultPlane {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)};
    auto planeTransform = plane.getTransformToSelf(defaultPlane);
    for (auto& vertex : vertices)
    {
      vertex = planeTransform * vertex;
    }

    return vertices;
  }

  std::vector<Vertex> createLine(const GeometryCore::Line& line, bool withArrowHead)
  {
    auto distance = glm::distance(line.start, line.end);
    auto defaultLineDir = glm::vec3(0.0f, distance, 0.0f);
    auto lineDir = line.end - line.start;
    auto crossProduct = glm::cross(defaultLineDir, lineDir);
    if (isEqual(crossProduct, glm::vec3(0.0f, 0.0f, 0.0f)) &&
        glm::dot(defaultLineDir, lineDir) < 0.0f)
    {
      distance = -distance;
    }

    std::vector<Vertex> vertices;
    Line defaultLine {glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, distance, 0.0f)};
    vertices.emplace_back(defaultLine.start, glm::vec3(0.0f, 0.0f, 1.0f));
    vertices.emplace_back(defaultLine.end, glm::vec3(0.0f, 0.0f, 1.0f));

    if (withArrowHead)
    {
      auto tangent = glm::tan(glm::radians(ARROW_HEAD_ANGLE));
      auto x = tangent * distance * ARROW_HEAD_LENGTH_COEF;
      auto y = defaultLine.end.y - (x / tangent);
      vertices.emplace_back(defaultLine.end, glm::vec3(0.0f, 0.0f, 1.0f));
      vertices.emplace_back(glm::vec3(-x, y, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
      vertices.emplace_back(defaultLine.end, glm::vec3(0.0f, 0.0f, 1.0f));
      vertices.emplace_back(glm::vec3(x, y, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    }

    auto transform = line.getTransformToSelf(defaultLine);
    for (auto& vertex : vertices)
    {
      vertex = transform * vertex;
    }

    return vertices;
  }

  std::vector<Vertex> createRay(const GeometryCore::Ray& ray, float length)
  {
    Vertex start(ray.origin, glm::vec3(0.0f));
    Vertex end(ray.origin + ray.direction * length, glm::vec3(0.0f));

    return {start, end};
  }

  std::vector<Vertex> getBRepCurveVertices(const NURBSCurve3D& curve, size_t segments)
  {
    std::vector<Vertex> vertices;
    for (size_t segmentIdx = 0; segmentIdx <= segments; ++segmentIdx)
    {
      float t = segmentIdx / float(segments);
      auto point = curve.getPoint(t);
      vertices.emplace_back(point, glm::vec3());
    }

    return vertices;
  }
}  // namespace MeshCore
