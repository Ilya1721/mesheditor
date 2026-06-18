#pragma once

#include <glm/glm.hpp>

#include "ShaderProgram.h"

using namespace std::filesystem;

namespace RenderSystem
{
  class TAAMotionVectorsShaderProgram : public ShaderProgram
  {
   public:
    TAAMotionVectorsShaderProgram(
      const path& vertexShaderPath, const path& fragmentShaderPath
    );

    void setView(const glm::mat4& view) const;
    void setProjection(const glm::mat4& projection) const;
    void setPrevModel(const glm::mat4& model) const;
    void setCurrModel(const glm::mat4& model) const;

   private:
    void initUniformLocations();

   private:
    int mPrevModel;
    int mCurrModel;
    int mPrevView;
    int mCurrView;
    int mProjection;

    mutable glm::mat4 mPrevViewMatrix;
  };
}  // namespace RenderSystem
