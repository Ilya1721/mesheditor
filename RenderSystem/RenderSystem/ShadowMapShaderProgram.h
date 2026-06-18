#pragma once

#include <glm/glm.hpp>

#include "ShaderProgram.h"

using namespace std::filesystem;

namespace RenderSystem
{
  class ShadowMapShaderProgram : public Object3DShaderProgram
  {
   public:
    ShadowMapShaderProgram(const path& vertexShaderPath, const path& fragmentShaderPath);

    void setModel(const glm::mat4& model) const override;
    void setMaterial(const Material& material) const override {};

    void setLightView(const glm::mat4& lightView) const;
    void setLightProjection(const glm::mat4& lightProjection) const;
    void setUseSkinningTransform(bool useSkinningTransform) const;
    void setSkinningTransforms(const std::vector<glm::mat4>& skinningTransforms) const;

   private:
    void initUniformLocations();

   protected:
    int mModel;
    int mLightView;
    int mLightProjection;
    int mUseSkinningTransform;
    int mSkinningTransforms;
  };
}  // namespace RenderSystem
