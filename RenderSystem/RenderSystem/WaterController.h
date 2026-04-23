#pragma once

#include <memory>

#include "CubemapTexture.h"
#include "ImageTexture.h"
#include "MeshRenderData.h"
#include "Water.h"

namespace RenderSystem
{
  class WaterController
  {
   public:
    WaterController();

    const ImageTexture& getNormalMap() const;
    const MeshRenderData& getPlaneRenderData() const;
    const WaterBlock& getWaterBlock() const;
    float getCurrentTime() const;
    bool isGeneratingWater() const;

    void updateWater(float lastFrameTime);
    void startGeneratingWater();
    void stopGeneratingWater();

   private:
    MeshRenderData createWaterPlane(float width, float length, int gridX, int gridZ)
      const;
    void initWaterBlock();
    void initPlane();

   private:
    WaterBlock mWaterBlock;
    MeshRenderData mPlaneRenderData;
    std::unique_ptr<ImageTexture> mNormalMap;
    bool mGenerateWater;
    float mCurrentTime;
  };
}
