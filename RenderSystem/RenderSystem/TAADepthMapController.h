#pragma once

#include "ControllerWithDepthMap.h"
#include "TAADepthMapShaderProgram.h"

namespace RenderSystem
{
  class TAADepthMapController : public ControllerWithDepthMap
  {
  public:
    TAADepthMapController(const path& vertexShaderPath, const path& fragmentShaderPath);

    TAADepthMapShaderProgram* getShaderProgram();

    void setModel(const glm::mat4& model);
    void setView(const glm::mat4& view);
    void setJitteredProjection(const glm::mat4& projection);

  private:
    TAADepthMapShaderProgram mShaderProgram;
  };
}

