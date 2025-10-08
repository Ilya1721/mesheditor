#pragma once

#include "ImageTexture.h"
#include "Object3DMaterialParams.h"

namespace RenderSystem
{
  class BlinnPhongMaterial
  {
  public:
    void init(int shaderProgram);
    void setParams(const BlinnPhongMaterialParamsExtended& params);

  private:
    void setAmbient(const float* ambient) const;
    void setDiffuse(const float* diffuse) const;
    void setSpecular(const float* specular) const;
    void setShininess(float shininess) const;
    void setDiffuseTexture(const ImageTexture& texture) const;
    void initUniformLocations();

    int getUniformLocation(const char* name) const;

  private:
    int mShaderProgram;
    int mAmbient;
    int mDiffuse;
    int mSpecular;
    int mShininess;
    int mDiffuseTexture;
    int mHasDiffuseTexture;
  };
}
