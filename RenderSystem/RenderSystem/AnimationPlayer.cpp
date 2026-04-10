#include "AnimationPlayer.h"

#include <glfw/glfw3.h>

#include "Scene.h"

namespace RenderSystem
{
  AnimationPlayer::AnimationPlayer(Scene* scene) : mScene(scene)
  {
  }

  void AnimationPlayer::onKeyPressed(int key)
  {
    if (key == GLFW_KEY_RIGHT)
    {
      mScene->nextAnimation();
    }
    else if (key == GLFW_KEY_LEFT)
    {
      mScene->prevAnimation();
    }
    else if (key == GLFW_KEY_S)
    {
      mScene->setObjectToAnimate(mScene->getPickedObject());
    }
    else if (key == GLFW_KEY_SPACE)
    {
      mScene->toggleAnimationPlaying();
    }
  }
}
