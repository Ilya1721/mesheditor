#pragma once

#include <glm/glm.hpp>

#include "ShaderProgram.h"
#include "Texture2D.h"

namespace RenderSystem
{
  class TAAResolveShaderProgram : public ShaderProgram
  {
   public:
    TAAResolveShaderProgram(
      const std::filesystem::path& vertexShaderPath,
      const std::filesystem::path& fragmentShaderPath
    );

    void preRenderSetup() const override;

    void setPrevDepthMap(const Texture2D& texture) const;
    void setCurrDepthMap(const Texture2D& texture) const;
    void setPrevColorTexture(const Texture2D& texture) const;
    void setCurrColorTexture(const Texture2D& texture) const;
    void setMotionVectorsTexture(const Texture2D& texture) const;
    void setIsFirstFrame(bool isFirstFrame) const;
    void setScreenSize(const glm::vec2& size) const;

   private:
    void initUniformLocations();

   private:
    mutable int mPrevDepthMapId;
    int mPrevDepthMapLocation;
    mutable int mCurrDepthMapId;
    int mCurrDepthMapLocation;
    mutable int mCurrColorTextureId;
    int mCurrColorTextureLocation;
    mutable int mPrevColorTextureId;
    int mPrevColorTextureLocation;
    mutable int mMotionVectorsTextureId;
    int mMotionVectorsTextureLocation;

    int mIsFirstFrame;
    int mScreenSize;
  };
}  // namespace RenderSystem
