#pragma once

#include <glm/glm.hpp>

namespace RenderSystem
{
  struct Particle
  {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 color;

    float sideLength;
    float rotation;
    float life;
    float maxLife;
    float frame;
  };
}
