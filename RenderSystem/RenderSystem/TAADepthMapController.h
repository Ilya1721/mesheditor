#pragma once

#include "DepthTexture.h"
#include "FrameBufferObject.h"
#include "TAADepthMapShaderProgram.h"

namespace RenderSystem
{
  class TAADepthMapController
  {
  public:
    TAADepthMapController(const path& vertexShaderPath, const path& fragmentShaderPath);

    const DepthTexture& getPrevDepthMap() const;
    const DepthTexture& getCurrDepthMap() const;

    void setModel(const glm::mat4& model);
    void setView(const glm::mat4& view);
    void setProjection(const glm::mat4& projection);
    void setScreenSize(int width, int height);
    void renderSceneToDepthMap(const std::function<void()>& renderSceneFunc);
    void swapDepthMaps();

  private:
    TAADepthMapShaderProgram mShaderProgram;
    DepthTexture mPrevDepthMap;
    DepthTexture mCurrDepthMap;
    FrameBufferObject mFBO;
  };
}

