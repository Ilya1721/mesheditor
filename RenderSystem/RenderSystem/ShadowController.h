#pragma once

#include <functional>

#include "DepthTexture.h"
#include "FrameBufferObject.h"
#include "ShadowMapShaderProgram.h"

namespace RenderSystem
{
  class ShadowController
  {
   public:
    ShadowController(const path& vertexShaderPath, const path& fragmentShaderPath);

    const DepthTexture& getDepthMap() const;
    ShadowMapShaderProgram* getShaderProgram();

    void setTextureDimensions(int width, int height);
    void renderSceneToDepthMap(const std::function<void()>& renderSceneFunc);

   private:
    void init();

   private:
    ShadowMapShaderProgram mShaderProgram;
    DepthTexture mTexture;
    FrameBufferObject mFBO;
  };
}  // namespace RenderSystem
