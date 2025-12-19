#pragma once

#include "AbstractShaderProgram.h"
#include "ShaderProgram.h"

using namespace std::filesystem;

namespace RenderSystem
{
  class TAADepthMapShaderProgram : public ShaderProgram, public AbstractShaderProgram
  {
  public:
    TAADepthMapShaderProgram(const path& vertexShaderPath, const path& fragmentShaderPath);
    virtual ~TAADepthMapShaderProgram() = default;

    void setModel(const glm::mat4& model) override;
    void setView(const glm::mat4& view);
    void setProjection(const glm::mat4& projection);

  private:
    void initUniformLocations();

  private:
    int mModel;
    int mView;
    int mProjection;
  };
}  // namespace RenderSystem
