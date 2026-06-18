#pragma once

#include <memory>

#include "CubemapTexture.h"
#include "Material.h"
#include "MeshRenderData.h"
#include "Object3D.h"

namespace RenderSystem
{
  class WaterController
  {
   public:
    WaterController();

    const Object3D* getWaterPlane() const;
    float getCurrentTime() const;
    bool isGeneratingWater() const;

    void updateWaterPlaneTransform(const glm::mat4& transform);
    void updateWater(float lastFrameTime);
    void startGeneratingWater();
    void stopGeneratingWater();

   private:
    std::unique_ptr<Object3D> createWaterPlane(
      float width, float length, int gridX, int gridZ
    ) const;
    void initMaterial();
    void initPlane();

   private:
    std::unique_ptr<Object3D> mPlane;
    WaterMaterial mWaterMaterial;
    bool mGenerateWater;
    float mCurrentTime;
  };
}
