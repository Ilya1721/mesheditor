#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include <unordered_map>
#include <vector>

struct Joint
{
  std::string name;
  int parentIndex;
  std::vector<int> childrenIndices;

  glm::vec3 translation;
  glm::quat rotation;
  glm::vec3 scale;

  glm::mat4 localMatrix;
  glm::mat4 globalMatrix;
  glm::mat4 inverseBindMatrix;
};

struct Skeleton
{
  std::vector<Joint> joints;
  int rootJointIndex;
  std::unordered_map<std::string, int> nameIndexMap;
};

template <typename T> struct Keyframe
{
  float time;
  T value;
};

enum class Interpolation
{
  Linear,
  Step,
  CubicSpline,
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
  int jointIndex;
  int samplerIndex;
  TransformComponent transformComponent;
};

struct Animation
{
  std::string name;
  float duration;
  std::vector<AnimationChannel> channels;

  std::vector<AnimationSampler<glm::vec3>> translationSamplers;
  std::vector<AnimationSampler<glm::quat>> rotationSamplers;
  std::vector<AnimationSampler<glm::vec3>> scaleSamplers;
};

struct Pose
{
  std::vector<glm::vec3> translations;
  std::vector<glm::quat> rotations;
  std::vector<glm::vec3> scales;
};