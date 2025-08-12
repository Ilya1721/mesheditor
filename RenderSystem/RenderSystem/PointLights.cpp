#include "PointLights.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glm/gtc/type_ptr.hpp>

#include "glad.h"

namespace RenderSystem
{
  void PointLights::init(int shaderProgram)
  {
    mShaderProgram = shaderProgram;
    initUniformLocations();
  }

  void PointLights::addLight(
    const PointLightParams& params, const Point3D& lightSourcePos
  )
  {
    PointLight light {};
    light.init(mShaderProgram);
    light.setParams(params);
    light.setLightSourcePos(glm::value_ptr(lightSourcePos));
    mLights.emplace(mLights.size(), std::move(light));
    glUniform1i(mLightsCount, mLights.size());
  }

  void PointLights::removeLight(unsigned int index)
  {
    auto pointLightIt = mLights.find(index);
    if (pointLightIt == mLights.end()) return;
    pointLightIt->second.setParams(PointLightParams {});
  }

  PointLight* PointLights::getLight(unsigned int index)
  {
    auto pointLightIt = mLights.find(index);
    return pointLightIt != mLights.end() ? &pointLightIt->second : nullptr;
  }

  void PointLights::initUniformLocations()
  {
    mLightsCount = getUniformLocation("pointLightsCount");
  }

  int PointLights::getUniformLocation(const char* name) const
  {
    return glGetUniformLocation(mShaderProgram, name);
  }
}  // namespace RenderSystem
