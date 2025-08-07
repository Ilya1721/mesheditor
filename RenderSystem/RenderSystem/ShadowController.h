#pragma once

#include <functional>

#include "AbstractShaderProgram.h"
#include "DepthTexture.h"
#include "FrameBufferObject.h"
#include "GeometryCore/Typedefs.h"
#include "ShadowMapShaderProgram.h"

using namespace GeometryCore;

namespace RenderSystem
{
  class ShadowController : public AbstractShaderProgram
  {
   public:
    ShadowController(const path& vertexShaderPath, const path& fragmentShaderPath);

    unsigned int getDepthTextureId() const;

    void setModel(const glm::mat4& model) override;
    void setLightView(const glm::mat4& lightView);
    void setLightProjection(const glm::mat4& lightProjection);
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
