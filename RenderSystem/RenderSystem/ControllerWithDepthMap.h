#pragma once

#include <functional>

#include "DepthTexture.h"
#include "FrameBufferObject.h"

namespace RenderSystem
{
  class ShaderProgram;

  class ControllerWithDepthMap
  {
  public:
    ControllerWithDepthMap() = default;
    virtual ~ControllerWithDepthMap() = default;

    ControllerWithDepthMap& operator=(ShaderProgram* shaderProgram);

    const DepthTexture& getDepthMap() const;

    void setDepthMapSize(int width, int height);
    void renderSceneToDepthMap(const std::function<void()>& renderSceneFunc);

  private:
    void init();

  protected:
    DepthTexture mTexture;
    FrameBufferObject mFBO;

  private:
    ShaderProgram* mShaderProgram = nullptr;
  };
}

