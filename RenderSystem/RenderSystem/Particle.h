#pragma once

#include <glm/glm.hpp>

namespace RenderSystem
{
  struct Emitter
  {
    glm::vec3 position {};
  };

  struct Particle
  {
    glm::vec3 position {};
    glm::vec3 velocity {};
    glm::vec4 colorMultiplier {};
    bool isActive = false;
    float sideLength {};
    float viewDirRotation {};
    float blendedFrameIdx {};
    float currentTime {};
    float duration {};
  };
}
