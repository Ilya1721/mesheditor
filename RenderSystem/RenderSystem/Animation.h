#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace RenderSystem
{
  struct Pose
  {
    glm::vec3 translation {};
    glm::quat rotation {};
    glm::vec3 scale {};
  };

  struct Joint
  {
    std::string name;
    int parentIndex = -1;
    std::vector<int> childrenIndices;

    Pose restPose;
    glm::mat4 localTransform {};
    glm::mat4 globalTransform {};
    glm::mat4 inverseGlobalTransform {};
  };

  struct Skeleton
  {
    std::vector<Joint> joints;
    int rootJointIndex = -1;
    std::unordered_map<std::string, int> nameIndexMap;
  };

  template <typename T> struct Keyframe
  {
    float time {};
    T value {};
  };

  enum class Interpolation
  {
    Linear,
    Step,
    Unknown
  };

  template <typename T> struct AnimationSampler
  {
    std::vector<Keyframe<T>> keyframes;
    Interpolation interpolation;
  };

  enum class TransformComponent
  {
    Translation,
    Rotation,
    Scale,
    Unknown
  };

  struct AnimationChannel
  {
    int jointIndex = -1;
    int samplerIndex = -1;
    TransformComponent transformComponent;
  };

  struct Animation
  {
    std::string name;
    float duration {};
    std::vector<AnimationChannel> channels;

    std::vector<AnimationSampler<glm::vec3>> translationSamplers;
    std::vector<AnimationSampler<glm::quat>> rotationSamplers;
    std::vector<AnimationSampler<glm::vec3>> scaleSamplers;
  };

  glm::mat4 calcJointLocalTransform(const Pose& pose);
}