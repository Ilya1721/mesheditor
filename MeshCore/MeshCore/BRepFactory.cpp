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
}
