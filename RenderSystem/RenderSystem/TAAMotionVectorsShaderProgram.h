#pragma once

#include <glm/glm.hpp>

#include "AbstractShaderProgram.h"
#include "ShaderProgram.h"

using namespace std::filesystem;

namespace RenderSystem
{
  class TAAMotionVectorsShaderProgram : public ShaderProgram, public AbstractShaderProgram
  {
  public:
    TAAMotionVectorsShaderProgram(const path& vertexShaderPath, const path& fragmentShaderPath);
    virtual ~TAAMotionVectorsShaderProgram() = default;

    void setModel(const glm::mat4& model) override;
    void setPrevView(const glm::mat4& view);
    void setCurrentView(const glm::mat4& view);
    void setProjection(const glm::mat4& projection);
    void setJitteredProjection(const glm::mat4& projection);

  private:
    void initUniformLocations();

  protected:
    int mModel;
    int mPrevView;
    int mCurrentView;
    int mProjection;
    int mJitteredProjection;
  };
}

