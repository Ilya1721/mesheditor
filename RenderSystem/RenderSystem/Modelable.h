#pragma once

#include <glm/glm.hpp>

namespace RenderSystem
{
  struct Modelable
  {
    virtual ~Modelable() = default;
    virtual void setModel(const glm::mat4& model) = 0;
  };
}  // namespace RenderSystem
