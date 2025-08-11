#pragma once

#include <unordered_map>

#include "LightParams.h"
#include "PointLight.h"

namespace RenderSystem
{
  class PointLights
  {
  public:
    void init(int shaderProgram);
    void addLight(const PointLightParams& params);
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
}

