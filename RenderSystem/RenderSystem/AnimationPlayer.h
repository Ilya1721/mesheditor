#pragma once

#include "Operation.h"

namespace RenderSystem
{
  class Scene;

  class AnimationPlayer : public Operation
  {
  public:
    AnimationPlayer(Scene* scene);

    void onKeyPressed(int key) override;

  private:
    Scene* mScene;
  };
}

