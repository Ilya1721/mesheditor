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
    virtual ~TAAMotionVectorsShaderProgram() = default;

    void setPrevModel(const glm::mat4& model);
    void setCurrentModel(const glm::mat4& model);
    void setPrevView(const glm::mat4& view);
    void setCurrentView(const glm::mat4& view);
    void setPrevJitteredProjection(const glm::mat4& projection);
    void setCurrentJitteredProjection(const glm::mat4& projection);

   private:
    void initUniformLocations();

   protected:
    int mPrevModel;
    int mCurrentModel;
    int mPrevView;
    int mCurrentView;
    int mPrevJitteredProjection;
    int mCurrentJitteredProjection;
  };
}  // namespace RenderSystem
