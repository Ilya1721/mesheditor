#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "Texture2D.h"

namespace RenderSystem
{
  class TAAController
  {
   public:
    const Texture2D* getPrevColorTexture() const;
    const Texture2D* getCurrColorTexture() const;
    const Texture2D* getPrevDepthTexture() const;
    const Texture2D* getCurrDepthTexture() const;
    const Texture2D* getMotionVectorsTexture() const;
    const Texture2D* getResolvedColorTexture() const;
    glm::mat4 getJitteredProjection() const;

    void update(
      const glm::mat4& projection, unsigned int viewportWidth, unsigned int viewportHeight
    );
    void swapTextures();

   private:
    mutable int mFrameIndex;
    glm::mat4 mProjection;
    int mScreenWidth;
    int mScreenHeight;
    std::shared_ptr<Texture2D> mPrevColorTexture;
    std::shared_ptr<Texture2D> mCurrColorTexture;
    std::shared_ptr<Texture2D> mPrevDepthMap;
    std::shared_ptr<Texture2D> mCurrDepthMap;
    std::shared_ptr<Texture2D> mMotionVectorsTexture;
    std::shared_ptr<Texture2D> mResolvedColorTexture;
  };
}  // namespace RenderSystem
