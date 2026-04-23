#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace RenderSystem
{
  struct Wave
  {
    float amplitude;
    float length;
    float speed;
    glm::vec2 direction;
  };

  struct WaterBlock
  {
    int fresnelPower;
    float depthFalloff;
    float reflectionIntensity;
    float normalStrength;
    glm::vec3 shallowColor;
    glm::vec3 deepColor;
    std::vector<glm::vec2> normalMapMoves;
    std::vector<Wave> waves;
  };
}
