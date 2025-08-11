#pragma once

#include <string>

namespace RenderSystem
{
  struct PointLightParams;

  class PointLight
  {
  public:
    void init(int shaderProgram);
    void setParams(const PointLightParams& light);
    void setLightSourcePos(const float* lightSourcePos) const;

  private:
    void setAmbient(const float* ambient) const;
    void setDiffuse(const float* diffuse) const;
    void setSpecular(const float* specular) const;
    void setConstant(float constant) const;
    void setLinear(float linear) const;
    void setQuadratic(float quadratic) const;

    void initUniformLocations();

    int getUniformLocation(const char* name) const;
    std::string getParamsUniformName(const std::string& paramName) const;

  private:
    unsigned int mIndex;
    int mShaderProgram;
    int mLightSourcePos;
    int mConstant;
    int mLinear;
    int mQuadratic;
    int mAmbient;
    int mDiffuse;
    int mSpecular;
  };
}  // namespace RenderSystem
