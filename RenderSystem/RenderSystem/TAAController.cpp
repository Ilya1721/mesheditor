#include "TAAController.h"

#include <glm/gtx/transform.hpp>

#include "Constants.h"
#include "Object3D.h"
#include "TextureFactory.h"
#include "Viewport.h"

namespace
{
  float halton(int index, int base)
  {
    float f = 1.0f, r = 0.0f;
    int i = index;
    while (i > 0)
    {
      f /= base;
      r += f * (i % base);
      i /= base;
    }
    return r;
  }

  glm::vec2 getJitter(int frameIndex)
  {
    return glm::vec2(halton(frameIndex, 2), halton(frameIndex, 3)) - 0.5f;
  }

  glm::vec2 convertJitterToNDC(const glm::vec2& jitter, int screenWidth, int screenHeight)
  {
    return glm::vec2(
      (jitter.x * 2.0f) / static_cast<float>(screenWidth),
      (jitter.y * 2.0f) / static_cast<float>(screenHeight)
    );
  }
}  // namespace

namespace RenderSystem
{
  const Texture2D* TAAController::getPrevColorTexture() const
  {
    return mPrevColorTexture.get();
  }

  const Texture2D* TAAController::getCurrColorTexture() const
  {
    return mCurrColorTexture.get();
  }

  const Texture2D* TAAController::getPrevDepthTexture() const
  {
    return mPrevDepthMap.get();
  }

  const Texture2D* TAAController::getCurrDepthTexture() const
  {
    return mCurrDepthMap.get();
  }

  const Texture2D* TAAController::getMotionVectorsTexture() const
  {
    return mMotionVectorsTexture.get();
  }

  const Texture2D* TAAController::getResolvedColorTexture() const
  {
    return mResolvedColorTexture.get();
  }

  void TAAController::update(
    const glm::mat4& projection, unsigned int viewportWidth, unsigned int viewportHeight
  )
  {
    mFrameIndex = 0;
    mScreenWidth = viewportWidth;
    mScreenHeight = viewportHeight;
    mProjection = projection;
    mPrevColorTexture = createColorTexture(viewportWidth, viewportHeight);
    mCurrColorTexture = createColorTexture(viewportWidth, viewportHeight);
    mPrevDepthMap = createDepthTexture(viewportWidth, viewportHeight);
    mCurrDepthMap = createDepthTexture(viewportWidth, viewportHeight);
    mMotionVectorsTexture = createMotionVectorsTexture(viewportWidth, viewportHeight);
    mResolvedColorTexture = createColorTexture(viewportWidth, viewportHeight);
  }

  void TAAController::swapTextures()
  {
    std::swap(mPrevDepthMap, mCurrDepthMap);
    std::swap(mPrevColorTexture, mResolvedColorTexture);
  }

  glm::mat4 TAAController::getJitteredProjection() const
  {
    auto jitter = getJitter(mFrameIndex + 1);
    auto jitterNDC = convertJitterToNDC(jitter, mScreenWidth, mScreenHeight);
    glm::vec3 jitterTranslationVec {jitterNDC.x, jitterNDC.y, 0.0f};
    auto jitterTranslation = glm::translate(glm::mat4(1.0f), jitterTranslationVec);
    mFrameIndex = (mFrameIndex + 1) % SAMPLE_COUNT_TAA;

    return jitterTranslation * mProjection;
  }
}  // namespace RenderSystem
