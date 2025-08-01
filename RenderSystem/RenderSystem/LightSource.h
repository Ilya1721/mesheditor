#pragma once

#include "GeometryCore/Typedefs.h"

using namespace GeometryCore;

namespace RenderSystem
{
  class SceneShaderProgram;
  class ShadowController;

  class LightSource
  {
   public:
    LightSource(
      SceneShaderProgram* sceneShaderProgram, ShadowController* shadowController
    );

    const Point3D& getPosition() const;
    void setPosition(const Point3D& pos);

   private:
    Point3D mPos;
    SceneShaderProgram* mSceneShaderProgram;
    ShadowController* mShadowController;
  };
}  // namespace RenderSystem
