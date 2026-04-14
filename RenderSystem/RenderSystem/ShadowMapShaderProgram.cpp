#include "ShadowMapShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

namespace RenderSystem
{
  ShadowMapShaderProgram::ShadowMapShaderProgram(
    const path& vertexShaderPath, const path& fragmentShaderPath
  )
    : ShaderProgram(vertexShaderPath, fragmentShaderPath),
      mModel(),
      mLightView(),
      mLightProjection()
  {
    initUniformLocations();
  }

  void ShadowMapShaderProgram::setModel(const glm::mat4& model)
  {
    invoke([this, &model]()
           { glUniformMatrix4fv(mModel, 1, false, glm::value_ptr(model)); });
  }

  void ShadowMapShaderProgram::setLightView(const glm::mat4& lightView)
  {
    invoke([this, &lightView]()
           { glUniformMatrix4fv(mLightView, 1, false, glm::value_ptr(lightView)); });
  }

  void ShadowMapShaderProgram::setLightProjection(const glm::mat4& lightProjection)
  {
    invoke(
      [this, &lightProjection]()
      { glUniformMatrix4fv(mLightProjection, 1, false, glm::value_ptr(lightProjection)); }
    );
  }

  void ShadowMapShaderProgram::setUseSkinningTransform(bool useSkinningTransform)
  {
    invoke([this, useSkinningTransform]()
           { glUniform1i(mUseSkinningTransform, useSkinningTransform); });
  }

  void ShadowMapShaderProgram::setSkinningTransforms(
    const std::vector<glm::mat4>& skinningTransforms
  )
  {
    if (skinningTransforms.empty())
    {
      return;
    }

    invoke(
      [this, &skinningTransforms]()
      {
        glUniformMatrix4fv(
          mSkinningTransforms,
          skinningTransforms.size(),
          GL_FALSE,
          glm::value_ptr(skinningTransforms[0])
        );
      }
    );
  }

  void ShadowMapShaderProgram::initUniformLocations()
  {
    mModel = glGetUniformLocation(mShaderProgram, "model");
    mLightView = glGetUniformLocation(mShaderProgram, "lightView");
    mLightProjection = glGetUniformLocation(mShaderProgram, "lightProjection");
    mUseSkinningTransform = getUniformLocation("useSkinningTransform");
    mSkinningTransforms = getUniformLocation("skinningTransforms");
  }
}  // namespace RenderSystem
