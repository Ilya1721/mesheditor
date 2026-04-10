#include "AnimationController.h"

#include "Object3D.h"

namespace
{
  using namespace RenderSystem;

  constexpr float EPSILON = 1e-5f;

  template <typename T>
  T linearInterpolation(
    const Keyframe<T>& left, const Keyframe<T>& right, float interpolationFactor
  )
  {
    if constexpr (std::is_same_v<T, glm::vec3>)
    {
      return glm::mix(left.value, right.value, interpolationFactor);
    }
    else if constexpr (std::is_same_v<T, glm::quat>)
    {
      return glm::slerp(left.value, right.value, interpolationFactor);
    }

    return T();
  }

  template <typename T>
  T interpolate(
    const Keyframe<T>& left,
    const Keyframe<T>& right,
    Interpolation interpolationMode,
    float interpolationFactor
  )
  {
    if (interpolationMode == Interpolation::Step)
    {
      return left.value;
    }
    else if (interpolationMode == Interpolation::Linear)
    {
      return linearInterpolation(left, right, interpolationFactor);
    }

    return T();
  }

  float getInterpolationFactor(float minTime, float maxTime, float currTime)
  {
    return (currTime - minTime) / (maxTime - minTime);
  }

  template <typename T>
  void updatePoseComponent(
    const AnimationSampler<T>& sampler, float currentTime, T& poseComponent
  )
  {
    for (size_t frameIdx = 0; frameIdx + 1 < sampler.keyframes.size(); ++frameIdx)
    {
      const auto& currKeyframe = sampler.keyframes[frameIdx];
      const auto& nextKeyframe = sampler.keyframes[frameIdx + 1];
      if (currentTime + EPSILON >= currKeyframe.time && currentTime < nextKeyframe.time)
      {
        auto interpolationFactor =
          getInterpolationFactor(currKeyframe.time, nextKeyframe.time, currentTime);
        poseComponent = interpolate(
          currKeyframe, nextKeyframe, sampler.interpolation, interpolationFactor
        );
      }
    }
  }
}

namespace RenderSystem
{
  AnimationController::AnimationController() : mObjectToAnimate(nullptr)
  {
    setObjectToAnimate(nullptr);
  }

  const std::vector<glm::mat4>& AnimationController::getSkinningTransforms() const
  {
    return mSkinningTransforms;
  }

  bool AnimationController::useSkinningTransforms() const
  {
    return (mObjectToAnimate && !mObjectToAnimate->getAnimations().empty());
  }

  void AnimationController::updateSkinningTransforms(float lastFrameTime)
  {
    if (!useSkinningTransforms() || !mPlayAnimation)
    {
      return;
    }

    updateCurrentTime(lastFrameTime);
    updatePoses();
    updateSkinningTransforms();
  }

  void AnimationController::toggleAnimationPlaying()
  {
    mPlayAnimation = !mPlayAnimation;
    if (!mPlayAnimation)
    {
      mCurrentTime = 0.0f;
    }
  }

  void AnimationController::nextAnimation()
  {
    if (mCurrentAnimIdx < mMaxAnimIdx)
    {
      resetAnimationPlaying();
      ++mCurrentAnimIdx;
    }
  }

  void AnimationController::prevAnimation()
  {
    if (mCurrentAnimIdx > 0)
    {
      resetAnimationPlaying();
      --mCurrentAnimIdx;
    }
  }

  void AnimationController::setObjectToAnimate(Object3D* object)
  {
    mObjectToAnimate = object;
    resetAnimationPlaying();
    resetJoints(object);
    setMaxAnimIdx(object);
  }

  void AnimationController::resetAnimationPlaying()
  {
    mPlayAnimation = false;
    mCurrentTime = 0.0f;
  }

  void AnimationController::resetJoints(Object3D* objectToAnimate)
  {
    if (objectToAnimate)
    {
      auto jointsSize = objectToAnimate->getSkeleton().joints.size();
      mPoses.clear();
      mPoses.resize(jointsSize);
      mSkinningTransforms.clear();
      mSkinningTransforms.resize(jointsSize, glm::mat4(1.0f));
    }
  }

  void AnimationController::updatePoses()
  {
    const auto& animations = mObjectToAnimate->getAnimations();
    const auto& animation = animations[mCurrentAnimIdx];
    for (const auto& channel : animation.translationChannels)
    {
      auto& pose = mPoses[channel.jointIndex];
      updatePoseComponent(channel.sampler, mCurrentTime, pose.translation);
    }
    for (const auto& channel : animation.rotationChannels)
    {
      auto& pose = mPoses[channel.jointIndex];
      updatePoseComponent(channel.sampler, mCurrentTime, pose.rotation);
    }
    for (const auto& channel : animation.scaleChannels)
    {
      auto& pose = mPoses[channel.jointIndex];
      updatePoseComponent(channel.sampler, mCurrentTime, pose.scale);
    }
  }

  void AnimationController::updateSkinningTransforms()
  {
    auto rootJointIdx = mObjectToAnimate->getSkeleton().rootJointIndex;
    updateSkinningTransforms(rootJointIdx, glm::mat4(1.0f));
  }

  void AnimationController::updateSkinningTransforms(
    size_t jointIdx, const glm::mat4& parentGlobalTransform
  )
  {
    const auto& joint = mObjectToAnimate->getSkeleton().joints[jointIdx];
    auto localTransform = calcJointLocalTransform(mPoses[jointIdx]);
    auto globalTransform = parentGlobalTransform * localTransform;
    mSkinningTransforms[jointIdx] = globalTransform * joint.inverseGlobalTransform;
    for (const auto& childIdx : joint.childrenIndices)
    {
      updateSkinningTransforms(childIdx, globalTransform);
    }
  }

  void AnimationController::updateCurrentTime(float lastFrameTime)
  {
    const auto& animations = mObjectToAnimate->getAnimations();
    const auto& animation = animations[mCurrentAnimIdx];
    mCurrentTime = fmodf(mCurrentTime + lastFrameTime, animation.duration);
  }

  void AnimationController::setMaxAnimIdx(Object3D* objectToAnimate)
  {
    if (!objectToAnimate)
    {
      mMaxAnimIdx = 0;
    }
    else
    {
      auto animationsSize = objectToAnimate->getAnimations().size();
      mMaxAnimIdx = animationsSize > 0 ? animationsSize - 1 : 0;
    }
  }
}