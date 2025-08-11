#pragma once

namespace RenderSystem
{
  struct DirectionalLightParams;

  class DirectionalLight
  {
   public:
    void init(int shaderProgram);
    void setLightSourcePos(const float* lightSourcePos) const;
    void setCameraPos(const float* cameraPos) const;
    void setParams(const DirectionalLightParams& light);

   private:
    void setAmbient(const float* ambient) const;
    void setDiffuse(const float* diffuse) const;
    void setSpecular(const float* specular) const;

    void initUniformLocations();
    void setupSettings();

    int getUniformLocation(const char* name) const;

   private:
    int mShaderProgram;
    int mLightSourcePos;
    int mCameraPos;
    int mAmbient;
    int mDiffuse;
    int mSpecular;
  };
}  // namespace RenderSystem
