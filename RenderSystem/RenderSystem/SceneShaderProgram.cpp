#include "SceneShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "Constants.h"
#include "glad/glad.h"

namespace RenderSystem
{
  SceneShaderProgram::SceneShaderProgram(
    const std::filesystem::path& vertexShaderPath,
    const std::filesystem::path& fragmentShaderPath
  )
    : ShaderProgram(vertexShaderPath, fragmentShaderPath),
      mModel(),
      mLightView(),
      mLightProjection(),
      mView(),
      mProjection(),
      mShadowBias(),
      mDepthMap()
  {
    mDirectionalLight.init(mShaderProgram);
    mPointLights.init(mShaderProgram);
    mMaterial.init(mShaderProgram);
    initUniformLocations();
    setUp();
  }

  void SceneShaderProgram::setModel(const glm::mat4& model)
  {
    invoke([this, &model]()
           { glUniformMatrix4fv(mModel, 1, false, glm::value_ptr(model)); });
  }

  void SceneShaderProgram::setDirLightSourcePos(const Point3D& lightSourcePos)
  {
    invoke([this, &lightSourcePos]()
           { mDirectionalLight.setLightSourcePos(glm::value_ptr(lightSourcePos)); });
  }

  void SceneShaderProgram::setCameraPos(const Point3D& cameraPos)
  {
    invoke([this, &cameraPos]()
           { mDirectionalLight.setCameraPos(glm::value_ptr(cameraPos)); });
  }

  void SceneShaderProgram::setMaterialParams(const MaterialParams& materialParams)
  {
    invoke([this, &materialParams]() { mMaterial.setParams(materialParams); });
  }

  void SceneShaderProgram::setDirectionalLightParams(const DirectionalLightParams& params)
  {
    invoke([this, &params]() { mDirectionalLight.setParams(params); });
  }

  void SceneShaderProgram::setView(const glm::mat4& view)
  {
    invoke([this, &view]() { glUniformMatrix4fv(mView, 1, false, glm::value_ptr(view)); }
    );
  }

  void SceneShaderProgram::setProjection(const glm::mat4& projection)
  {
    invoke([this, &projection]()
           { glUniformMatrix4fv(mProjection, 1, false, glm::value_ptr(projection)); });
  }

  void SceneShaderProgram::setDepthMap(int textureId)
  {
    invoke(
      [this, textureId]()
      {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glUniform1i(mDepthMap, 1);
      }
    );
  }

  void SceneShaderProgram::setLightView(const glm::mat4& lightView)
  {
    invoke([this, &lightView]()
           { glUniformMatrix4fv(mLightView, 1, false, glm::value_ptr(lightView)); });
  }

  void SceneShaderProgram::setLightProjection(const glm::mat4& lightProjection)
  {
    invoke(
      [this, &lightProjection]()
      { glUniformMatrix4fv(mLightProjection, 1, false, glm::value_ptr(lightProjection)); }
    );
  }

  PointLight* SceneShaderProgram::addPointLight(
    const PointLightParams& params, const Point3D& lightSourcePos
  )
  {
    return mPointLights.addLight(params, lightSourcePos);
  }

  void SceneShaderProgram::removePointLight(unsigned int index)
  {
    mPointLights.removeLight(index);
  }

  void SceneShaderProgram::setPointLightParams(
    unsigned int index, const PointLightParams& params
  )
  {
    const auto& light = mPointLights.getLight(index);
    if (light) { light->setParams(params); }
  }

  void SceneShaderProgram::setPointLightSourcePos(
    unsigned int index, const Point3D& lightSourcePos
  )
  {
    const auto& light = mPointLights.getLight(index);
    if (light) { light->setLightSourcePos(glm::value_ptr(lightSourcePos)); }
  }

  void SceneShaderProgram::initUniformLocations()
  {
    mModel = glGetUniformLocation(mShaderProgram, "model");
    mView = glGetUniformLocation(mShaderProgram, "view");
    mProjection = glGetUniformLocation(mShaderProgram, "projection");
    mShadowBias = glGetUniformLocation(mShaderProgram, "shadowBias");
    mDepthMap = glGetUniformLocation(mShaderProgram, "depthMap");
    mLightView = glGetUniformLocation(mShaderProgram, "lightView");
    mLightProjection = glGetUniformLocation(mShaderProgram, "lightProjection");
  }

  void SceneShaderProgram::setUp()
  {
    invoke([this]() { glUniform1f(mShadowBias, SHADOW_BIAS); });
  }
}  // namespace RenderSystem
