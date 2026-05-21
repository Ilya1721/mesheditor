#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace MeshCore
{
  struct SurfaceDomain
  {
    float uMin, uMax;
    float vMin, vMax;
  };

  class BRepSurface
  {
   public:
    virtual glm::vec3 getPoint(float u, float v) = 0;
    virtual glm::vec3 getNormal(float u, float v) = 0;
  };

  class BRepNURBSSurface : public BRepSurface
  {
   public:
    glm::vec3 getPoint(float u, float v) override;
    glm::vec3 getNormal(float u, float v) override;

   private:
    int mSmoothnessLevelU;
    int mSmoothnessLevelV;
    std::vector<float> mBoundariesU;
    std::vector<float> mBoundariesV;
    std::vector<std::vector<glm::vec4>> mControlPoints;
    std::vector<std::vector<float>> mWeights;
  };
}
