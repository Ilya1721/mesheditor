#pragma once

#include "ControllerWithDepthMap.h"
#include "ShadowMapShaderProgram.h"
#include "ViewportListener.h"

namespace RenderSystem
{
  class ShadowController : public ControllerWithDepthMap, public ViewportListener
  {
   public:
    ShadowController(const path& vertexShaderPath, const path& fragmentShaderPath);

    ShadowMapShaderProgram* getShaderProgram();

    void onViewportChanged(Viewport* viewport) override;

    void setModel(const glm::mat4& model);
    void setLightView(const glm::mat4& lightView);
    void setLightProjection(const glm::mat4& lightProjection);

   private:
    ShadowMapShaderProgram mShaderProgram;
  };
}  // namespace RenderSystem
