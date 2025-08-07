#pragma once

#include <glm/glm.hpp>

namespace RenderSystem
{
  struct AbstractShaderProgram
  {
    virtual ~AbstractShaderProgram() = default;
    virtual void setModel(const glm::mat4& model) = 0;
  };
}  // namespace RenderSystem
