#include "WaterController.h"

#include "Constants.h"
#include "MeshCore/Vertex.h"

using namespace MeshCore;

namespace RenderSystem
{
  WaterController::WaterController() : mGenerateWater(false), mCurrentTime(0.0f)
  {
    initWaterBlock();
    initPlane();
  }

  const ImageTexture& WaterController::getNormalMap() const
  {
    return *mNormalMap;
  }

  const MeshRenderData& WaterController::getPlaneRenderData() const
  {
    return mPlaneRenderData;
  }

  const WaterBlock& WaterController::getWaterBlock() const
  {
    return mWaterBlock;
  }

  float WaterController::getCurrentTime() const
  {
    return mCurrentTime;
  }

  bool WaterController::isGeneratingWater() const
  {
    return mGenerateWater;
  }

  void WaterController::updateWater(float lastFrameTime)
  {
    mCurrentTime += lastFrameTime;
  }

  void WaterController::startGeneratingWater()
  {
    mGenerateWater = true;
  }

  void WaterController::stopGeneratingWater()
  {
    mGenerateWater = false;
    mCurrentTime = 0.0f;
  }

  void WaterController::initWaterBlock()
  {
    mNormalMap = std::make_unique<ImageTexture>(WATER_NORMAL_MAP_PATH);
    mWaterBlock.fresnelPower = 5;
    mWaterBlock.depthFalloff = 0.05f;
    mWaterBlock.reflectionIntensity = 0.5f;
    mWaterBlock.normalStrength = 0.6f;
    mWaterBlock.shallowColor = glm::vec3(0.0f, 0.8f, 0.7f);
    mWaterBlock.deepColor = glm::vec3(0.0f, 0.2f, 0.5f);
    mWaterBlock.normalMapMoves.emplace_back(0.05f, 0.02f);
    mWaterBlock.normalMapMoves.emplace_back(-0.03f, 0.04f);
    mWaterBlock.waves = {
      {0.5f, 20.0f, 1.0f, glm::normalize(glm::vec2(1.0f, 0.3f))},
      {0.3f, 10.0f, 1.5f, glm::normalize(glm::vec2(-0.7f, 0.8f))},
      {0.2f, 5.0f, 2.0f, glm::normalize(glm::vec2(0.2f, 1.0f))}
    };
  }

  MeshRenderData WaterController::createWaterPlane(
    float width, float length, int gridX, int gridZ
  ) const
  {
    MeshRenderData renderData;

    auto maxGridX = gridX - 1;
    auto maxGridZ = gridZ - 1;
    float stepX = width / (gridX - 1);
    float stepZ = length / (gridZ - 1);

    for (int zIdx = 0; zIdx < maxGridZ; zIdx++)
    {
      for (int xIdx = 0; xIdx < maxGridX; xIdx++)
      {
        auto nextXIdx = xIdx + 1;
        auto nextZIdx = zIdx + 1;

        float x0 = -width * 0.5f + xIdx * stepX;
        float x1 = -width * 0.5f + nextXIdx * stepX;
        float z0 = -length * 0.5f + zIdx * stepZ;
        float z1 = -length * 0.5f + nextZIdx * stepZ;

        float u0 = static_cast<float>(xIdx) / maxGridX;
        float u1 = static_cast<float>(nextXIdx) / maxGridX;
        float v0 = static_cast<float>(zIdx) / maxGridZ;
        float v1 = static_cast<float>(nextZIdx) / maxGridZ;

        glm::vec3 normal(0.0f, 1.0f, 0.0f);

        renderData.append({glm::vec3(x0, 0, z0), normal, glm::vec2(u0, v0)});
        renderData.append({glm::vec3(x0, 0, z1), normal, glm::vec2(u0, v1)});
        renderData.append({glm::vec3(x1, 0, z0), normal, glm::vec2(u1, v0)});

        renderData.append({glm::vec3(x1, 0, z0), normal, glm::vec2(u1, v0)});
        renderData.append({glm::vec3(x0, 0, z1), normal, glm::vec2(u0, v1)});
        renderData.append({glm::vec3(x1, 0, z1), normal, glm::vec2(u1, v1)});
      }
    }

    return renderData;
  }

  void WaterController::initPlane()
  {
    mPlaneRenderData = createWaterPlane(5.0f, 5.0f, 100, 100);
  }
}
