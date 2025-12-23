#include "SceneShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include "Camera.h"
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
      mJitteredProjection(),
      mShadowBias(),
      mShadowMap()
  {
    mDirectionalLight.init(mShaderProgram);
    mPointLights.init(mShaderProgram);
    mBlinnPhongMaterial.init(mShaderProgram);
    mGlassMaterial.init(mShaderProgram);
    initUniformLocations();
    setUp();
  }

  void SceneShaderProgram::onCameraPosChanged(Camera* camera)
  {
    setCameraPos(camera->getEye());
    setView(camera->getViewMatrix());
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

  void SceneShaderProgram::setJitteredProjection(const glm::mat4& projection)
  {
    invoke(
      [this, &projection]()
      { glUniformMatrix4fv(mJitteredProjection, 1, false, glm::value_ptr(projection)); }
    );
  }

  void SceneShaderProgram::setShadowMap(const DepthTexture& texture) const
  {
    invoke([this, &texture]() { texture.passToFragmentShader(mShadowMap, 1); });
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
    invoke([this, &texture]() { texture.passToFragmentShader(mSkybox, 2); });
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

  void SceneShaderProgram::setUVScale(const glm::vec2& scale)
  {
    invoke([this, &scale]() { glUniform2fv(mUVScale, 1, glm::value_ptr(scale)); });
  }

  void SceneShaderProgram::initUniformLocations()
  {
    mModel = getUniformLocation("model");
    mView = getUniformLocation("view");
    mJitteredProjection = getUniformLocation("jitteredProjection");
    mShadowBias = getUniformLocation("shadowBias");
    mShadowMap = getUniformLocation("shadowMap");
    mLightView = getUniformLocation("lightView");
    mLightProjection = getUniformLocation("lightProjection");
    mSkybox = getUniformLocation("skybox");
    mMaterialType = getUniformLocation("material.type");
    mUVScale = getUniformLocation("uvScale");
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
