#include "Viewport.h"

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glm/gtc/matrix_transform.hpp>

#include "Constants.h"
#include "SceneShaderProgram.h"
#include "glad/glad.h"

namespace RenderSystem
{
  Viewport::Viewport(
    int width,
    int height,
    const MeshCore::AABBox* rootBBox,
    SceneShaderProgram* sceneShaderProgram
  )
    : mFov(FOV),
      mNearPlaneDistance(NEAR_PLANE_DISTANCE),
      mFarPlaneDistance(FAR_PLANE_DISTANCE),
      mPos(VIEWPORT_POSITION.x, VIEWPORT_POSITION.y),
      mWidth(width),
      mHeight(height),
      mProjectionType(PROJECTION_TYPE::PERSPECTIVE),
      mProjectionMatrix(1.0f),
      mRootBBox(rootBBox),
      mSceneShaderProgram(sceneShaderProgram),
      mBBoxViewportGapCoef(BBOX_VIEWPORT_GAP_COEF)
  {
  }

  glm::mat4 Viewport::createProjectionMatrix() const
  {
    auto aspectRatio = mWidth / mHeight;

    if (mProjectionType == PROJECTION_TYPE::ORTHOGRAPHIC)
    {
      float height = mRootBBox->getHeight();
      float width = height * aspectRatio;

      return glm::ortho(
        -width, width, -height, height, mNearPlaneDistance, mFarPlaneDistance
      );
    }

    return glm::perspective(
      glm::radians(mFov), aspectRatio, mNearPlaneDistance, mFarPlaneDistance
    );
  }

  void Viewport::invokeEditOperation(const std::function<void()>& action)
  {
    action();
    mProjectionMatrix = createProjectionMatrix();
    mSceneShaderProgram->setProjection(mProjectionMatrix);
  }

  void Viewport::setProjectionType(PROJECTION_TYPE projectionType)
  {
    invokeEditOperation([this, &projectionType]() { mProjectionType = projectionType; });
  }

  void Viewport::resize(int width, int height)
  {
    invokeEditOperation(
      [this, width, height]()
      {
        mWidth = width;
        mHeight = height;
        glViewport(mPos.x, mPos.y, mWidth, mHeight);
      }
    );
  }

  void Viewport::zoom(float step)
  {
    invokeEditOperation([this, &step]() { mBBoxViewportGapCoef += step; });
  }

  const glm::mat4& Viewport::getProjectionMatrix() const { return mProjectionMatrix; }

  const PROJECTION_TYPE Viewport::getProjectionType() const { return mProjectionType; }

  float Viewport::getFov() const { return mFov; }

  float Viewport::getNearPlaneDistance() const { return mNearPlaneDistance; }

  float Viewport::getFarPlaneDistance() const { return mFarPlaneDistance; }

  float Viewport::getWidth() const { return mWidth; }

  float Viewport::getHeight() const { return mHeight; }

  const glm::ivec2& Viewport::getPos() const { return mPos; }
}  // namespace RenderSystem
