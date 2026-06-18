#include "WaterController.h"

#include "Constants.h"
#include "MeshCore/Vertex.h"
#include "TextureFactory.h"

using namespace MeshCore;

namespace RenderSystem
{
  WaterController::WaterController() : mGenerateWater(false), mCurrentTime(0.0f)
  {
    initMaterial();
    initPlane();
  }

  const Object3D* WaterController::getWaterPlane() const
  {
    return mPlane.get();
  }

  float WaterController::getCurrentTime() const
  {
    return mCurrentTime;
  }

  bool WaterController::isGeneratingWater() const
  {
    return mGenerateWater;
  }

  void WaterController::updateWaterPlaneTransform(const glm::mat4& transform)
  {
    mPlane->updateTransform(transform);
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

  void WaterController::initMaterial()
  {
    mWaterMaterial.fresnelPower = 5;
    mWaterMaterial.depthFalloff = 0.05f;
    mWaterMaterial.reflectionIntensity = 0.75f;
    mWaterMaterial.normalStrength = 0.75f;
    mWaterMaterial.minFresnel = 0.3f;
    mWaterMaterial.shininess = 256.0f;
    mWaterMaterial.shallowColor = glm::vec3(0.0f, 0.8f, 0.7f);
    mWaterMaterial.deepColor = glm::vec3(0.0f, 0.2f, 0.5f);
    mWaterMaterial.normalMapMoves.emplace_back(0.05f, 0.02f);
    mWaterMaterial.normalMapMoves.emplace_back(-0.03f, 0.04f);
    mWaterMaterial.waves = {
      {0.5f, 20.0f, 1.0f, glm::normalize(glm::vec2(1.0f, 0.3f))},
      {0.3f, 10.0f, 1.5f, glm::normalize(glm::vec2(-0.7f, 0.8f))},
      {0.2f, 5.0f, 2.0f, glm::normalize(glm::vec2(0.2f, 1.0f))}
    };
  }

  std::unique_ptr<Object3D> WaterController::createWaterPlane(
    float width, float length, int gridX, int gridZ
  ) const
  {
    std::vector<Vertex> vertices;
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

        vertices.push_back({glm::vec3(x0, 0, z0), normal, glm::vec2(u0, v0)});
        vertices.push_back({glm::vec3(x0, 0, z1), normal, glm::vec2(u0, v1)});
        vertices.push_back({glm::vec3(x1, 0, z0), normal, glm::vec2(u1, v0)});

        vertices.push_back({glm::vec3(x1, 0, z0), normal, glm::vec2(u1, v0)});
        vertices.push_back({glm::vec3(x0, 0, z1), normal, glm::vec2(u0, v1)});
        vertices.push_back({glm::vec3(x1, 0, z1), normal, glm::vec2(u1, v1)});
      }
    }

    auto mesh = std::make_unique<Mesh>(vertices, false);
    return std::make_unique<Object3D>(std::move(mesh), mWaterMaterial);
  }

  void WaterController::initPlane()
  {
    mPlane = createWaterPlane(5.0f, 5.0f, 100, 100);
  }
}
