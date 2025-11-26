#pragma once

#include "ControllerWithDepthMap.h"
#include "ShadowMapShaderProgram.h"

namespace RenderSystem
{
  class ShadowController : public ControllerWithDepthMap
  {
   public:
    ShadowController(const path& vertexShaderPath, const path& fragmentShaderPath);

    ShadowMapShaderProgram* getShaderProgram();

    void setModel(const glm::mat4& model);
    void setLightView(const glm::mat4& lightView);
    void setLightProjection(const glm::mat4& lightProjection);

   private:
    ShadowMapShaderProgram mShaderProgram;
  };
}  // namespace RenderSystem
