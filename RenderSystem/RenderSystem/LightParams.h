#pragma once

#include <glm/glm.hpp>

namespace RenderSystem
{
  struct DirectionalLightParams
  {
    glm::vec3 ambient {};
    glm::vec3 diffuse {};
    glm::vec3 specular {};
  };

  struct PointLightParams
  {
    glm::vec3 ambient {};
    glm::vec3 diffuse {};
    glm::vec3 specular {};

    float constant {};
    float linear {};
    float quadratic {};
  };
};  // namespace RenderSystem
