#pragma once

#include <unordered_map>

#include "GeometryCore/Typedefs.h"
#include "LightParams.h"
#include "PointLight.h"

using namespace GeometryCore;

namespace RenderSystem
{
  class PointLights
  {
   public:
    void init(int shaderProgram);
    PointLight* addLight(const PointLightParams& params, const Point3D& lightSourcePos);
    void removeLight(unsigned int index);
    PointLight* getLight(unsigned int index);

   private:
    void initUniformLocations();
    int getUniformLocation(const char* name) const;

   private:
    int mShaderProgram;
    int mLightsCount;
    std::unordered_map<unsigned int, PointLight> mLights;
  };
}  // namespace RenderSystem
