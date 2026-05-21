#include "BRepFactory.h"

namespace MeshCore
{
  NURBSCurve3D createBRepCircle(float radius, const glm::vec3& normal)
  {
    std::vector<glm::vec4> controlPoints = {{1, 0, 0, 1},  {1, 1, 0, 1},  {0, 1, 0, 1},
                                            {-1, 1, 0, 1}, {-1, 0, 0, 1}, {-1, -1, 0, 1},
                                            {0, -1, 0, 1}, {1, -1, 0, 1}, {1, 0, 0, 1}};
    float s = std::sqrt(2.0f) / 2.0f;
    std::vector<float> weights = {1, s, 1, s, 1, s, 1, s, 1};
    std::vector<float> boundaries = {0,    0,     0,     0.25f, 0.25f, 0.5f,
                                     0.5f, 0.75f, 0.75f, 1,     1,     1};
    int smoothnessLevel = 2;

    return NURBSCurve3D(smoothnessLevel, boundaries, controlPoints, weights);
  }
}
