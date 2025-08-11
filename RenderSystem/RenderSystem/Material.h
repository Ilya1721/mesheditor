#pragma once

#include "MaterialParams.h"

namespace RenderSystem
{
  class Material
  {
  public:
    void init(int shaderProgram);
    void setParams(const MaterialParams& material);

  private:
    void setAmbient(const float* ambient) const;
    void setDiffuse(const float* diffuse) const;
    void setSpecular(const float* specular) const;
    void setShininess(float shininess) const;

    void initUniformLocations();
    void setupSettings();

    int getUniformLocation(const char* name) const;

  private:
    int mShaderProgram;
    int mAmbient;
    int mDiffuse;
    int mSpecular;
    int mShininess;
  };
}
