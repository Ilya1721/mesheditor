#include "PointLights.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace RenderSystem
{
  void PointLights::init(int shaderProgram)
  {
    mShaderProgram = shaderProgram;
    initUniformLocations();
  }

  PointLight* PointLights::addLight(
    const PointLightParams& params, const glm::vec3& lightSourcePos
  )
  {
    auto index = mLights.size();
    PointLight light {};
    light.init(mShaderProgram, index);
    light.setParams(params);
    light.setLightSourcePos(glm::value_ptr(lightSourcePos));

    mLights.emplace(index, std::move(light));
    glUniform1i(mLightsCount, mLights.size());

    return getLight(index);
  }

  void PointLights::removeLight(unsigned int index)
  {
    mLights.erase(index);
    glUniform1i(mLightsCount, mLights.size());
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
