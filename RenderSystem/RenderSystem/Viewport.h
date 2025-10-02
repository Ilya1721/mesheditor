#pragma once

#include <functional>
#include <glm/glm.hpp>

#include "MeshCore/AABBox.h"
#include "Utility/CallbackMechanism.h"

namespace RenderSystem
{
  using namespace Utility;

  enum class PROJECTION_TYPE
  {
    ORTHOGRAPHIC,
    PERSPECTIVE
  };

  using viewportChangedCallback = void();

  class Viewport
  {
   public:
    Viewport(int width, int height, const MeshCore::AABBox* rootBBox);

    void setProjectionType(PROJECTION_TYPE projectionType);
    void resize(int width, int height);
    void zoom(float step);
    void addOnViewportChangedCallback(
      const std::function<viewportChangedCallback>& callback
    );

    float getFov() const;
    float getNearPlaneDistance() const;
    float getFarPlaneDistance() const;
    float getWidth() const;
    float getHeight() const;
    const glm::ivec2& getPos() const;
    const glm::mat4& getProjectionMatrix() const;
    const PROJECTION_TYPE getProjectionType() const;

   private:
    void invokeEditOperation(const std::function<void()>& action);
    glm::mat4 createProjectionMatrix() const;

   private:
    float mFov;
    float mNearPlaneDistance;
    float mFarPlaneDistance;
    float mWidth;
    float mHeight;
    float mBBoxViewportGapCoef;

    glm::ivec2 mPos;
    PROJECTION_TYPE mProjectionType;
    glm::mat4 mProjectionMatrix;
    const MeshCore::AABBox* mRootBBox;
    CallbackMechanism<viewportChangedCallback> mViewportChangedCallbacks;
  };
}  // namespace RenderSystem
