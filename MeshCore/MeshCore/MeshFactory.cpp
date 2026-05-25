#include "MeshFactory.h"

#include <cmath>
#include <numbers>
#include <vector>

#include "BRepCurve.h"
#include "Constants.h"

using namespace std::numbers;

namespace MeshCore
{
  std::vector<Vertex> createUnitSphereAtOrigin()
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

        float posX = sinVertical * sinHorizontal;
        float posY = cosVertical;
        float posZ = sinVertical * cosHorizontal;

        Vertex vertex;
        vertex.pos = glm::vec3(posX, posY, posZ);
        vertex.normal = vertex.pos;
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

  std::vector<Vertex> createUnitCubeAtOrigin()
  {
    float halfSide = 0.5f;
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

  std::vector<Vertex> createUnitXYCircle(size_t segments)
  {
    std::vector<Vertex> vertices;
    auto step = glm::two_pi<float>() / segments;
    for (size_t idx = 0; idx <= segments; ++idx)
    {
      auto x = cosf(idx * step);
      auto y = sinf(idx * step);
      vertices.emplace_back(glm::vec3(x, y, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    }

    return vertices;
  }

  std::vector<Vertex> createUnitXYPlane()
  {
    std::vector<Vertex> vertices;
    auto halfWidth = 0.5f;
    auto halfLength = 0.5f;
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

    return vertices;
  }

  std::vector<Vertex> createUnitXLine(bool withArrowHead)
  {
    std::vector<Vertex> vertices;
    vertices.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f));
    vertices.emplace_back(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f));

    if (withArrowHead)
    {
      vertices.emplace_back(glm::vec3(0.8f, 0.2f, 0.0f), glm::vec3(0.0f));
      vertices.emplace_back(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f));
      vertices.emplace_back(glm::vec3(0.8f, -0.2f, 0.0f), glm::vec3(0.0f));
      vertices.emplace_back(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f));
    }

    return vertices;
  }

  std::vector<Vertex> getBRepCurveVertices(const NURBSCurve3D& curve, size_t segments)
  {
    std::vector<Vertex> vertices;
    for (size_t segmentIdx = 0; segmentIdx <= segments; ++segmentIdx)
    {
      auto t = std::min(segmentIdx / float(segments), 1.0f - EPSILON);
      auto point = curve.getPoint(t);
      vertices.emplace_back(point, glm::vec3(0.0f));
    }

    return vertices;
  }

  std::vector<Vertex> getBRepSurfaceVertices(
    const NURBSSurface& surface, size_t resolutionU, size_t resolutionV
  )
  {
    std::vector<Vertex> vertices;

    for (size_t x = 0; x < resolutionU; ++x)
    {
      auto tx0 = static_cast<float>(x) / resolutionU;
      auto tx1 = static_cast<float>(x + 1) / resolutionU;
      auto u0 = surface.getAdjustedU(tx0);
      auto u1 = surface.getAdjustedU(std::min(tx1, 1.0f - EPSILON));
      for (size_t y = 0; y < resolutionV; ++y)
      {
        auto ty0 = static_cast<float>(y) / resolutionV;
        auto ty1 = static_cast<float>(y + 1) / resolutionV;
        auto v0 = surface.getAdjustedV(ty0);
        auto v1 = surface.getAdjustedV(std::min(ty1, 1.0f - EPSILON));

        glm::vec3 p00 = surface.getPoint(u0, v0);
        glm::vec3 p10 = surface.getPoint(u1, v0);
        glm::vec3 p01 = surface.getPoint(u0, v1);
        glm::vec3 p11 = surface.getPoint(u1, v1);

        glm::vec3 n00 = surface.getNormal(u0, v0);
        glm::vec3 n10 = surface.getNormal(u1, v0);
        glm::vec3 n01 = surface.getNormal(u0, v1);
        glm::vec3 n11 = surface.getNormal(u1, v1);

        vertices.push_back({p00, n00});
        vertices.push_back({p01, n01});
        vertices.push_back({p10, n10});

        vertices.push_back({p10, n10});
        vertices.push_back({p01, n01});
        vertices.push_back({p11, n11});
      }
    }

    return vertices;
  }
}  // namespace MeshCore
