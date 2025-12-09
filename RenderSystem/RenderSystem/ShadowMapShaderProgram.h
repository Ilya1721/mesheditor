#pragma once

#include <glm/glm.hpp>

#include "ShaderProgram.h"

using namespace std::filesystem;

namespace RenderSystem
{
  class ShadowMapShaderProgram : public ShaderProgram
  {
   public:
    ShadowMapShaderProgram(const path& vertexShaderPath, const path& fragmentShaderPath);
    virtual ~ShadowMapShaderProgram() = default;

    void setModel(const glm::mat4& model);
    void setLightView(const glm::mat4& lightView);
    void setLightProjection(const glm::mat4& lightProjection);

   private:
    void initUniformLocations();

   protected:
    int mModel;
    int mLightView;
    int mLightProjection;
  };
}  // namespace RenderSystem
