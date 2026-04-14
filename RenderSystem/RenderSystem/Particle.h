#pragma once

#include <glm/glm.hpp>

namespace RenderSystem
{
  struct Particle
  {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 colorMultiplier;

    float sideLength;
    float viewDirRotation;
    float blendedFrameIdx;
    float currentTime;
    float duration;
  };
}
