#pragma once

#include <glm/glm.hpp>

#include "DepthTexture.h"
#include "ShaderProgram.h"
#include "TAAColorTexture.h"
#include "TAAMotionVectorsTexture.h"

namespace RenderSystem
{
  class TAAResolveShaderProgram : public ShaderProgram
  {
   public:
    TAAResolveShaderProgram(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );

    void setCurrentColorTexture(const TAAColorTexture& texture) const;
    void setPreviousColorTexture(const TAAColorTexture& texture) const;
    void setPrevDepthMap(const DepthTexture& texture) const;
    void setCurrDepthMap(const DepthTexture& texture) const;
    void setMotionVectorsTexture(const TAAMotionVectorsTexture& texture) const;
    void setIsFirstFrame(bool isFirstFrame);
    void setScreenSize(const glm::vec2& size);

   private:
    void initUniformLocations();

  private:
    int mCurrentColorTexture;
    int mPreviousColorTexture;
    int mPrevDepthMap;
    int mCurrDepthMap;
    int mMotionVectorsTexture;
    int mIsFirstFrame;
    int mScreenSize;
  };
}  // namespace RenderSystem
