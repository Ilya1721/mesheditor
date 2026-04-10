#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Animation.h"

namespace RenderSystem
{
  class Object3D;

  class AnimationController
  {
   public:
    AnimationController();

    const std::vector<glm::mat4>& getSkinningTransforms() const;
    bool useSkinningTransforms() const;

    void updateSkinningTransforms(float lastFrameTime);
    void toggleAnimationPlaying();
    void nextAnimation();
    void prevAnimation();
    void setObjectToAnimate(Object3D* object);

   private:
    void resetAnimationPlaying();
    void resetJoints(Object3D* objectToAnimate);
    void updatePoses();
    void updateSkinningTransforms();
    void updateSkinningTransforms(
      size_t jointIdx, const glm::mat4& parentGlobalTransform
    );
    void updateCurrentTime(float lastFrameTime);
    void setMaxAnimIdx(Object3D* objectToAnimate);

   private:
    Object3D* mObjectToAnimate;
    size_t mCurrentAnimIdx;
    size_t mMaxAnimIdx;
    bool mPlayAnimation;
    float mCurrentTime;
    std::vector<Pose> mPoses;
    std::vector<glm::mat4> mSkinningTransforms;
  };
}
