#include "BRepFactory.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace MeshCore
{
  NURBSCurve3D createUnitXYBRepCircle()
  {
    std::vector<glm::vec4> controlPoints = {{1, 0, 0, 1},  {1, 1, 0, 1},  {0, 1, 0, 1},
                                            {-1, 1, 0, 1}, {-1, 0, 0, 1}, {-1, -1, 0, 1},
                                            {0, -1, 0, 1}, {1, -1, 0, 1}, {1, 0, 0, 1}};
    float diagonalWeight = std::sqrt(2.0f) / 2.0f;
    std::vector<float> weights = {1, diagonalWeight, 1, diagonalWeight, 1, diagonalWeight,
                                  1, diagonalWeight, 1};
    std::vector<float> boundaries = {0,    0,     0,     0.25f, 0.25f, 0.5f,
                                     0.5f, 0.75f, 0.75f, 1,     1,     1};
    int smoothnessLevel = 2;

    return NURBSCurve3D(smoothnessLevel, boundaries, controlPoints, weights);
  }

  NURBSSurface createUnitXYBRepPlane()
  {
    NURBSSurface surface;
    float halfWidth = 0.5f;
    float halfHeight = 0.5f;
    std::vector<std::vector<glm::vec4>> controlPoints = {
      std::vector<glm::vec4> {
        glm::vec4(-halfWidth, -halfHeight, 0.0f, 1.0f),
        glm::vec4(-halfWidth, halfHeight, 0.0f, 1.0f)
      },
      std::vector<glm::vec4> {
        glm::vec4(halfWidth, -halfHeight, 0.0f, 1.0f),
        glm::vec4(halfWidth, halfHeight, 0.0f, 1.0f)
      }
    };
    surface.setControlPoints(controlPoints);
    std::vector<std::vector<float>> weights = {
      std::vector<float> {1.0f, 1.0f}, std::vector<float> {1.0f, 1.0f}
    };
    surface.setWeights(weights);
    std::vector<float> boundariesU {0, 0, 1, 1};
    std::vector<float> boundariesV {0, 0, 1, 1};
    surface.setBoundaries(boundariesU, boundariesV);
    surface.setSmoothnessLevels(1, 1);

    return surface;
  }

  NURBSSurface createUnitBRepCylinder()
  {
    const auto U = 9;
    const auto V = 2;
    const auto diagonalWeight = std::sqrt(2.0f) / 2.0f;

    struct CircleControlPoint
    {
      float x;
      float y;
      float weight;
    };

    const CircleControlPoint circleControlPoints[U] = {
      {1.0f, 0.0f, 1.0f},  {1.0f, 1.0f, diagonalWeight},
      {0.0f, 1.0f, 1.0f},  {-1.0f, 1.0f, diagonalWeight},
      {-1.0f, 0.0f, 1.0f}, {-1.0f, -1.0f, diagonalWeight},
      {0.0f, -1.0f, 1.0f}, {1.0f, -1.0f, diagonalWeight},
      {1.0f, 0.0f, 1.0f}
    };

    std::vector<std::vector<glm::vec4>> controlPoints(U);
    std::vector<std::vector<float>> weights(U);

    for (size_t idx = 0; idx < U; ++idx)
    {
      controlPoints[idx].resize(V);
      weights[idx].resize(V);
      const auto& circleControlPoint = circleControlPoints[idx];

      controlPoints[idx][0] = {circleControlPoint.x, circleControlPoint.y, 0.0f, 1.0f};
      weights[idx][0] = circleControlPoint.weight;
      controlPoints[idx][1] = {circleControlPoint.x, circleControlPoint.y, 1.0f, 1.0f};
      weights[idx][1] = circleControlPoint.weight;
    }

    std::vector<float> boundariesU = {0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 4};
    std::vector<float> boundariesV = {0, 0, 1, 1};

    NURBSSurface surface;
    surface.setBoundaries(boundariesU, boundariesV);
    surface.setControlPoints(controlPoints);
    surface.setWeights(weights);
    surface.setSmoothnessLevels(2, 1);

    return surface;
  }
}
