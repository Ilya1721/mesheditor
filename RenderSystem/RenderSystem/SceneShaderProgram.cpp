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
    mBlinnPhongMaterial.init(mShaderProgram);
    mGlassMaterial.init(mShaderProgram);
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

  void SceneShaderProgram::setBlinnPhongMaterialParams(
    const BlinnPhongMaterialParamsExtended& params
  )
  {
    invoke([this, &params]() { mBlinnPhongMaterial.setParams(params); });
  }

  void SceneShaderProgram::setGlassMaterialParams(const GlassMaterialParams& params)
  {
    invoke([this, &params]() { mGlassMaterial.setParams(params); });
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

  void SceneShaderProgram::setDepthMap(const DepthTexture& texture) const
  {
    invoke([this, &texture]()
           { texture.passToFragmentShader(mDepthMap, DEPTH_TEXTURE_SLOT); });
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

  void SceneShaderProgram::setMaterialType(const MaterialType& materialType)
  {
    invoke([this, &materialType]()
           { glUniform1i(mMaterialType, static_cast<int>(materialType)); });
  }

  void SceneShaderProgram::setSkyboxCubemap(const CubemapTexture& texture) const
  {
    invoke([this, &texture]()
           { texture.passToFragmentShader(mSkybox, SKYBOX_TEXTURE_SLOT); });
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

  void SceneShaderProgram::initUniformLocations()
  {
    mModel = getUniformLocation("model");
    mView = getUniformLocation("view");
    mProjection = getUniformLocation("projection");
    mShadowBias = getUniformLocation("shadowBias");
    mDepthMap = getUniformLocation("depthMap");
    mLightView = getUniformLocation("lightView");
    mLightProjection = getUniformLocation("lightProjection");
    mSkybox = getUniformLocation("skybox");
    mMaterialType = getUniformLocation("material.type");
  }

  int SceneShaderProgram::getUniformLocation(const char* name) const
  {
    return glGetUniformLocation(mShaderProgram, name);
  }

  void SceneShaderProgram::setShadowBias(float shadowBias)
  {
    invoke([this, &shadowBias]() { glUniform1f(mShadowBias, shadowBias); });
  }

  void SceneShaderProgram::setUp()
  {
    setShadowBias(SHADOW_BIAS);
    setMaterialType(MaterialType::BLINN_PHONG);
  }
}  // namespace RenderSystem
