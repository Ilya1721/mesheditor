#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "BRepDomain.h"

namespace MeshCore
{
  class BRepSurface
  {
   public:
    virtual ~BRepSurface() = default;
    virtual glm::vec3 getPoint(float u, float v) const = 0;
    virtual glm::vec3 getNormal(float u, float v) const = 0;
    virtual SurfaceDomain getDomain() const = 0;
    virtual void applyTransform(const glm::mat4& transform) = 0;
  };

  class NURBSSurface : public BRepSurface
  {
   public:
    glm::vec3 getPoint(float u, float v) const override;
    glm::vec3 getNormal(float u, float v) const override;
    SurfaceDomain getDomain() const override;
    void applyTransform(const glm::mat4& transform) override;

    void setSmoothnessLevels(int smoothnessLevelU, int smoothnessLevelV);
    void setBoundaries(
      const std::vector<float>& boundariesU, const std::vector<float>& boundariesV
    );
    void setControlPoints(const std::vector<std::vector<glm::vec4>>& controlPoints);
    void setWeights(const std::vector<std::vector<float>>& weights);

   private:
    int mSmoothnessLevelU {};
    int mSmoothnessLevelV {};
    std::vector<float> mBoundariesU;
    std::vector<float> mBoundariesV;
    std::vector<std::vector<glm::vec4>> mControlPoints;
    std::vector<std::vector<float>> mWeights;
  };
}
