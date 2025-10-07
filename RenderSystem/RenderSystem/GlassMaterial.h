#pragma once

#include "MeshCore/MaterialParams.h"

using namespace MeshCore;

namespace RenderSystem
{
  class GlassMaterial
  {
   public:
    void init(int shaderProgram);
    void setParams(const GlassMaterialParams& params);

   private:
    void setRefractiveIndex(float refractiveIndex) const;
    void setReflectionStrength(float reflectionStrength) const;
    void setTransparency(float transparency) const;
    void setInterpolationFactor(float interpolationFactor) const;
    void setColor(const float* color) const;
    void initUniformLocations();
    int getUniformLocation(const char* name) const;

   private:
    int mShaderProgram;
    int mRefractiveIndex;
    int mReflectionStrength;
    int mTransparency;
    int mInterpolationFactor;
    int mColor;
  };
}  // namespace RenderSystem
