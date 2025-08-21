#include "MeshFactory.h"

#include <cmath>
#include <numbers>
#include <vector>

using namespace std::numbers;

namespace MeshCore
{
  std::unique_ptr<Mesh> createSphere(float radius)
  {
    std::vector<Vertex> tempVertices;
    constexpr size_t horizontalDivisions = 3;
    constexpr size_t verticalDivisions = 3;

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

    return std::make_unique<Mesh>(vertices);
  }
}  // namespace MeshCore
