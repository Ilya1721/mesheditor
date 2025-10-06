#include "MeshFactory.h"

#include <cmath>
#include <numbers>
#include <vector>

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
        vertex.pos = Vector3D(posX, posY, posZ);
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
      Point3D(-halfSide, -halfSide, -halfSide), Vector3D(-1.0f, -1.0f, -1.0f),
      Point2D(0.0f, 0.0f)
    );
    uniqueVertices.emplace_back(
      Point3D(halfSide, -halfSide, -halfSide), Vector3D(1.0f, -1.0f, -1.0f),
      Point2D(1.0f, 0.0f)
    );
    uniqueVertices.emplace_back(
      Point3D(halfSide, halfSide, -halfSide), Vector3D(1.0f, 1.0f, -1.0f),
      Point2D(1.0f, 1.0f)
    );
    uniqueVertices.emplace_back(
      Point3D(-halfSide, halfSide, -halfSide), Vector3D(-1.0f, 1.0f, -1.0f),
      Point2D(0.0f, 1.0f)
    );
    uniqueVertices.emplace_back(
      Point3D(-halfSide, -halfSide, halfSide), Vector3D(-1.0f, -1.0f, 1.0f),
      Point2D(0.0f, 0.0f)
    );
    uniqueVertices.emplace_back(
      Point3D(halfSide, -halfSide, halfSide), Vector3D(1.0f, -1.0f, 1.0f),
      Point2D(1.0f, 0.0f)
    );
    uniqueVertices.emplace_back(
      Point3D(halfSide, halfSide, halfSide), Vector3D(1.0f, 1.0f, 1.0f),
      Point2D(1.0f, 1.0f)
    );
    uniqueVertices.emplace_back(
      Point3D(-halfSide, halfSide, halfSide), Vector3D(-1.0f, 1.0f, 1.0f),
      Point2D(0.0f, 1.0f)
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
}  // namespace MeshCore
