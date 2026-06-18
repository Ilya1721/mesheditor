#include "PBRShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include "Camera.h"
#include "Material.h"

namespace
{
  constexpr int BASE_COLOR_TEXTURE_UNIT = 0;
  constexpr int NORMAL_MAP_TEXTURE_UNIT = 1;
  constexpr int METALLIC_ROUGHNESS_TEXTURE_UNIT = 2;
}

namespace RenderSystem
{
  PBRShaderProgram::PBRShaderProgram(
    const std::filesystem::path& vertexShaderPath,
    const std::filesystem::path& fragmentShaderPath
  )
    : Object3DShaderProgram(vertexShaderPath, fragmentShaderPath)
  {
    initUniformLocations();
  }

  void PBRShaderProgram::setMaterial(const Material& material) const
  {
    bind();
    const auto& pbrMaterial = static_cast<const PBRMaterial&>(material);
    glUniform1f(mMetallic, pbrMaterial.metallic);
    glUniform1f(mRoughness, pbrMaterial.rougness);
    glUniform3fv(mBaseColor, 1, glm::value_ptr(pbrMaterial.baseColor));
    glUniform2fv(mUVScale, 1, glm::value_ptr(pbrMaterial.uvScale));
    setBaseColorTexture(pbrMaterial.baseColorTexture.get());
    setNormalTexture(pbrMaterial.normalMap.get());
    setMetallicRoughnessTexture(pbrMaterial.metallicRougnessTexture.get());
  }

  void PBRShaderProgram::preRenderSetup() const
  {
    if (getFlagFromShader(mHasBaseColorTexture))
    {
      glBindTextureUnit(BASE_COLOR_TEXTURE_UNIT, mBaseColorTextureId);
    }
    if (getFlagFromShader(mHasNormalMapTexture))
    {
      glBindTextureUnit(NORMAL_MAP_TEXTURE_UNIT, mNormalMapTextureId);
    }
    if (getFlagFromShader(mHasMetallicRoughnessTexture))
    {
      glBindTextureUnit(METALLIC_ROUGHNESS_TEXTURE_UNIT, mMetallicRoughnessTextureId);
    }
  }

  void PBRShaderProgram::onCameraChanged(const Camera* camera) const
  {
    setView(camera->getViewMatrix());
    setCameraPos(camera->getEye());
  }

  void PBRShaderProgram::setModel(const glm::mat4& model) const
  {
    bind();
    glUniformMatrix4fv(mModel, 1, false, glm::value_ptr(model));
  }

  void PBRShaderProgram::setView(const glm::mat4& view) const
  {
    bind();
    glUniformMatrix4fv(mView, 1, false, glm::value_ptr(view));
  }

  void PBRShaderProgram::setProjection(const glm::mat4& projection) const
  {
    bind();
    glUniformMatrix4fv(mProjection, 1, false, glm::value_ptr(projection));
  }

  void PBRShaderProgram::setCameraPos(const glm::vec3& cameraPos) const
  {
    bind();
    glUniform3fv(mCameraPos, 1, glm::value_ptr(cameraPos));
  }

  void PBRShaderProgram::setLightPos(const glm::vec3& lightPos) const
  {
    bind();
    glUniform3fv(mLightPos, 1, glm::value_ptr(lightPos));
  }

  void PBRShaderProgram::setLightColor(const glm::vec3& lightColor) const
  {
    bind();
    glUniform3fv(mLightColor, 1, glm::value_ptr(lightColor));
  }

  void PBRShaderProgram::setBaseColorTexture(const Texture2D* texture) const
  {
    if (texture)
    {
      mBaseColorTextureId = texture->getId();
      glUniform1i(mHasBaseColorTexture, true);
      glUniform1i(mBaseColorTextureLocation, BASE_COLOR_TEXTURE_UNIT);
    }
    else
    {
      glUniform1i(mHasBaseColorTexture, false);
    }
  }

  void PBRShaderProgram::setNormalTexture(const Texture2D* texture) const
  {
    if (texture)
    {
      mNormalMapTextureId = texture->getId();
      glUniform1i(mHasNormalMapTexture, true);
      glUniform1i(mNormalMapTextureLocation, NORMAL_MAP_TEXTURE_UNIT);
    }
    else
    {
      glUniform1i(mHasNormalMapTexture, false);
    }
  }

  void PBRShaderProgram::setMetallicRoughnessTexture(const Texture2D* texture) const
  {
    if (texture)
    {
      mMetallicRoughnessTextureId = texture->getId();
      glUniform1i(mHasMetallicRoughnessTexture, true);
      glUniform1i(mMetallicRoughnessTextureLocation, METALLIC_ROUGHNESS_TEXTURE_UNIT);
    }
    else
    {
      glUniform1i(mHasMetallicRoughnessTexture, false);
    }
  }

  void PBRShaderProgram::setUseSkinningTransform(bool useSkinningTransform) const
  {
    bind();
    glUniform1i(mUseSkinningTransform, useSkinningTransform);
  }

  void PBRShaderProgram::setSkinningTransforms(
    const std::vector<glm::mat4>& skinningTransforms
  ) const
  {
    if (!skinningTransforms.empty())
    {
      bind();
      glUniformMatrix4fv(
        mSkinningTransforms,
        skinningTransforms.size(),
        GL_FALSE,
        glm::value_ptr(skinningTransforms[0])
      );
    }
  }

  void PBRShaderProgram::initUniformLocations()
  {
    mModel = getUniformLocation("model");
    mView = getUniformLocation("view");
    mProjection = getUniformLocation("projection");
    mUVScale = getUniformLocation("uvScale");
    mCameraPos = getUniformLocation("cameraPos");
    mLightPos = getUniformLocation("lightPos");
    mLightColor = getUniformLocation("lightColor");
    mBaseColorTextureLocation = getUniformLocation("baseColorTexture");
    mBaseColor = getUniformLocation("baseColor");
    mHasBaseColorTexture = getUniformLocation("hasBaseColorTexture");
    mNormalMapTextureLocation = getUniformLocation("normalTexture");
    mHasNormalMapTexture = getUniformLocation("hasNormalTexture");
    mMetallicRoughnessTextureLocation = getUniformLocation("metallicRoughnessTexture");
    mHasMetallicRoughnessTexture = getUniformLocation("hasMetallicRoughnessTexture");
    mUseSkinningTransform = getUniformLocation("useSkinningTransform");
    mSkinningTransforms = getUniformLocation("skinningTransforms");
  }
}  // namespace RenderSystem
