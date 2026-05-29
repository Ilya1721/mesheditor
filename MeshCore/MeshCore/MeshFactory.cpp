#include "MeshFactory.h"

#include <cmath>
#include <numbers>
#include <vector>

#include "BRepCurve.h"
#include "Constants.h"

using namespace std::numbers;

namespace
{
  using namespace MeshCore;

  std::pair<float, float> getT0T1(size_t t, size_t resolution, float min, float max)
  {
    auto t0 = static_cast<float>(t) / resolution;
    auto t1 = static_cast<float>(t + 1) / resolution;
    auto td0 = tFromDomain(t0, min, max);
    auto td1 = tFromDomain(std::min(t1, 1.0f - EPSILON), min, max);

    return std::make_pair(td0, td1);
  }

  bool isCellInsideFace(const BRepFace& face, float u0, float u1, float v0, float v1)
  {
    glm::vec2 uv00(u0, v0);
    glm::vec2 uv10(u1, v0);
    glm::vec2 uv01(u0, v1);
    glm::vec2 uv11(u1, v1);

    bool inside00 = face.isPointInside(uv00);
    bool inside10 = face.isPointInside(uv10);
    bool inside01 = face.isPointInside(uv01);
    bool inside11 = face.isPointInside(uv11);

    return inside00 && inside10 && inside01 && inside11;
  }

  std::vector<Vertex> getCellVertices(
    const BRepSurface& surface, float su0, float su1, float sv0, float sv1
  )
  {
    std::vector<Vertex> vertices;

    glm::vec3 p00 = surface.getPoint(su0, sv0);
    glm::vec3 p10 = surface.getPoint(su1, sv0);
    glm::vec3 p01 = surface.getPoint(su0, sv1);
    glm::vec3 p11 = surface.getPoint(su1, sv1);

    glm::vec3 n00 = surface.getNormal(su0, sv0);
    glm::vec3 n10 = surface.getNormal(su1, sv0);
    glm::vec3 n01 = surface.getNormal(su0, sv1);
    glm::vec3 n11 = surface.getNormal(su1, sv1);

    vertices.push_back({p00, n00});
    vertices.push_back({p01, n01});
    vertices.push_back({p10, n10});

    vertices.push_back({p10, n10});
    vertices.push_back({p01, n01});
    vertices.push_back({p11, n11});

    return vertices;
  }
}

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

  std::vector<Vertex> getBRepCurveVertices(const Curve3D& curve, size_t segments)
  {
    std::vector<Vertex> vertices;
    for (size_t segmentIdx = 0; segmentIdx <= segments; ++segmentIdx)
    {
      auto t = std::min(segmentIdx / static_cast<float>(segments), 1.0f - EPSILON);
      auto point = curve.getPoint(t);
      vertices.emplace_back(point, glm::vec3(0.0f));
    }

    return vertices;
  }

  std::vector<Vertex> getBRepSurfaceVertices(
    const BRepSurface& surface, size_t resolutionU, size_t resolutionV
  )
  {
    std::vector<Vertex> vertices;
    auto domain = surface.getDomain();

    for (size_t x = 0; x < resolutionU; ++x)
    {
      for (size_t y = 0; y < resolutionV; ++y)
      {
        auto [su0, su1] = getT0T1(x, resolutionU, domain.uMin, domain.uMax);
        auto [sv0, sv1] = getT0T1(y, resolutionV, domain.vMin, domain.vMax);
        auto cellVertices = getCellVertices(surface, su0, su1, sv0, sv1);
        vertices.insert(vertices.end(), cellVertices.begin(), cellVertices.end());
      }
    }

    return vertices;
  }

  std::vector<Vertex> getBRepFaceVertices(
    const BRepFace& face, size_t resolutionU, size_t resolutionV
  )
  {
    std::vector<Vertex> vertices;
    auto faceDomain = face.getDomain();
    auto surfaceDomain = face.surface->getDomain();

    for (size_t x = 0; x < resolutionU; ++x)
    {
      for (size_t y = 0; y < resolutionV; ++y)
      {
        auto [fu0, fu1] = getT0T1(x, resolutionU, faceDomain.uMin, faceDomain.uMax);
        auto [fv0, fv1] = getT0T1(y, resolutionV, faceDomain.vMin, faceDomain.vMax);

        if (!isCellInsideFace(face, fu0, fu1, fv0, fv1))
        {
          continue;
        }

        auto [su0, sv0] =
          face.uvMapper->faceToSurfaceUV(faceDomain, surfaceDomain, fu0, fv0);
        auto [su1, sv1] =
          face.uvMapper->faceToSurfaceUV(faceDomain, surfaceDomain, fu1, fv1);
        auto cellVertices = getCellVertices(*face.surface, su0, su1, sv0, sv1);
        vertices.insert(vertices.end(), cellVertices.begin(), cellVertices.end());
      }
    }

    return vertices;
  }

  std::vector<Vertex> getBRepModelVertices(const BRepModel& model)
  {
    std::vector<Vertex> vertices;
    for (const auto& face : model.faces)
    {
      auto faceVertices = getBRepFaceVertices(*face, 32, 32);
      vertices.insert(vertices.end(), faceVertices.begin(), faceVertices.end());
    }

    return vertices;
  }
}  // namespace MeshCore
