#include "MeshFactory.h"

#include <cmath>
#include <vector>

namespace MeshCore
{
  // Generates a UV sphere mesh with a given radius
  std::unique_ptr<Mesh> createSphere(float radius)
  {
    constexpr int latitudeSegments = 16;
    constexpr int longitudeSegments = 32;
    std::vector<Vertex> vertices;

    for (int lat = 0; lat < latitudeSegments; ++lat)
    {
      float theta1 = float(lat) * glm::pi<float>() / float(latitudeSegments);
      float theta2 = float(lat + 1) * glm::pi<float>() / float(latitudeSegments);
      for (int lon = 0; lon < longitudeSegments; ++lon)
      {
        float phi1 = float(lon) * 2.0f * glm::pi<float>() / float(longitudeSegments);
        float phi2 = float(lon + 1) * 2.0f * glm::pi<float>() / float(longitudeSegments);

        // Points on the sphere
        glm::vec3 p1 = glm::vec3(
          std::sin(theta1) * std::cos(phi1),
          std::cos(theta1),
          std::sin(theta1) * std::sin(phi1)
        );
        glm::vec3 p2 = glm::vec3(
          std::sin(theta2) * std::cos(phi1),
          std::cos(theta2),
          std::sin(theta2) * std::sin(phi1)
        );
        glm::vec3 p3 = glm::vec3(
          std::sin(theta2) * std::cos(phi2),
          std::cos(theta2),
          std::sin(theta2) * std::sin(phi2)
        );
        glm::vec3 p4 = glm::vec3(
          std::sin(theta1) * std::cos(phi2),
          std::cos(theta1),
          std::sin(theta1) * std::sin(phi2)
        );

        // Scale by radius
        glm::vec3 v1 = radius * p1;
        glm::vec3 v2 = radius * p2;
        glm::vec3 v3 = radius * p3;
        glm::vec3 v4 = radius * p4;

        // Normals are just the normalized positions for a sphere
        glm::vec3 n1 = glm::normalize(p1);
        glm::vec3 n2 = glm::normalize(p2);
        glm::vec3 n3 = glm::normalize(p3);
        glm::vec3 n4 = glm::normalize(p4);

        // Each quad is split into two triangles
        if (lat != 0)
        {
          vertices.emplace_back(v1, n1);
          vertices.emplace_back(v2, n2);
          vertices.emplace_back(v4, n4);
        }
        if (lat != latitudeSegments - 1)
        {
          vertices.emplace_back(v2, n2);
          vertices.emplace_back(v3, n3);
          vertices.emplace_back(v4, n4);
        }
      }
    }
    return std::make_unique<Mesh>(vertices);
  }
}  // namespace MeshCore
