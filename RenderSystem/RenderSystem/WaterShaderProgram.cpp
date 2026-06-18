#include "WaterShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include "Camera.h"
#include "Viewport.h"

namespace
{
  constexpr int SKYBOX_UNIT = 0;
  constexpr int NORMAL_MAP_UNIT = 1;
}

namespace RenderSystem
{
  WaterShaderProgram::WaterShaderProgram(
    const std::filesystem::path& vertexShaderPath,
    const std::filesystem::path& fragmentShaderPath
  )
    : Object3DShaderProgram(vertexShaderPath, fragmentShaderPath)
  {
    initUniformLocations();
  }

  void WaterShaderProgram::preRenderSetup() const
  {
    glBindTextureUnit(SKYBOX_UNIT, mSkyboxId);
    glBindTextureUnit(NORMAL_MAP_UNIT, mNormalMapId);
  }

  void WaterShaderProgram::onCameraChanged(const Camera* camera) const
  {
    setCameraPos(camera->getEye());
    setView(camera->getViewMatrix());
  }

  void WaterShaderProgram::initUniformLocations()
  {
    mModel = getUniformLocation("model");
    mView = getUniformLocation("view");
    mProjection = getUniformLocation("projection");
    mVTime = getUniformLocation("vTime");
    mFTime = getUniformLocation("fTime");
    mWaveCount = getUniformLocation("waveCount");
    mSkyboxLocation = getUniformLocation("skybox");
    mNormalMapLocation = getUniformLocation("normalMap");
    mNormalMapMoves = getUniformLocation("normalMapMoves");
    mNormalMapMoveCount = getUniformLocation("normalMapMoveCount");
    mNormalStrength = getUniformLocation("normalStrength");
    mDepthFalloff = getUniformLocation("depthFalloff");
    mFresnelPower = getUniformLocation("fresnelPower");
    mReflectionIntensity = getUniformLocation("reflectionIntensity");
    mCameraPos = getUniformLocation("cameraPos");
    mLightPos = getUniformLocation("lightPos");
    mLightColor = getUniformLocation("lightColor");
    mDeepColor = getUniformLocation("deepColor");
    mShallowColor = getUniformLocation("shallowColor");
    mMinFresnel = getUniformLocation("minFresnel");
    mShininess = getUniformLocation("shininess");
  }

  void WaterShaderProgram::setModel(const glm::mat4& model) const
  {
    bind();
    glUniformMatrix4fv(mModel, 1, false, glm::value_ptr(model));
  }

  void WaterShaderProgram::setMaterial(const Material& material) const
  {
    bind();
    const auto& waterMaterial = static_cast<const WaterMaterial&>(material);
    setWaves(waterMaterial.waves);
    glUniform1i(mNormalMapMoveCount, waterMaterial.normalMapMoves.size());
    glUniform2fv(
      mNormalMapMoves, waterMaterial.normalMapMoves.size(),
      glm::value_ptr(waterMaterial.normalMapMoves[0])
    );
    glUniform1f(mNormalStrength, waterMaterial.normalStrength);
    glUniform1f(mDepthFalloff, waterMaterial.depthFalloff);
    glUniform1f(mFresnelPower, waterMaterial.fresnelPower);
    glUniform1f(mReflectionIntensity, waterMaterial.reflectionIntensity);
    glUniform1f(mMinFresnel, waterMaterial.minFresnel);
    glUniform1f(mShininess, waterMaterial.shininess);
    glUniform3fv(mDeepColor, 1, glm::value_ptr(waterMaterial.deepColor));
    glUniform3fv(mShallowColor, 1, glm::value_ptr(waterMaterial.shallowColor));
  }

  void WaterShaderProgram::setCameraPos(const glm::vec3& cameraPos) const
  {
    bind();
    glUniform3fv(mCameraPos, 1, glm::value_ptr(cameraPos));
  }

  void WaterShaderProgram::setView(const glm::mat4& view) const
  {
    bind();
    glUniformMatrix4fv(mView, 1, false, glm::value_ptr(view));
  }

  void WaterShaderProgram::setProjection(const glm::mat4& projection) const
  {
    bind();
    glUniformMatrix4fv(mProjection, 1, false, glm::value_ptr(projection));
  }

  void WaterShaderProgram::setVTime(float time) const
  {
    bind();
    glUniform1f(mVTime, time);
  }

  void WaterShaderProgram::setFTime(float time) const
  {
    bind();
    glUniform1f(mFTime, time);
  }

  void WaterShaderProgram::setWaves(const std::vector<Wave>& waves) const
  {
    glUniform1i(mWaveCount, waves.size());
    for (int waveIdx = 0; waveIdx < waves.size(); waveIdx++)
    {
      std::string waveName = "waves[" + std::to_string(waveIdx) + "]";
      auto amplitude = getUniformLocation((waveName + ".amplitude").c_str());
      glUniform1f(amplitude, waves[waveIdx].amplitude);
      auto waveLength = getUniformLocation((waveName + ".waveLength").c_str());
      glUniform1f(waveLength, waves[waveIdx].length);
      auto speed = getUniformLocation((waveName + ".speed").c_str());
      glUniform1f(speed, waves[waveIdx].speed);
      auto direction = getUniformLocation((waveName + ".direction").c_str());
      glUniform2fv(direction, 1, glm::value_ptr(waves[waveIdx].direction));
    }
  }

  void WaterShaderProgram::setSkyboxCubemap(const CubemapTexture& texture) const
  {
    bind();
    mSkyboxId = texture.getId();
    glUniform1i(mSkyboxLocation, SKYBOX_UNIT);
  }

  void WaterShaderProgram::setNormalMap(const Texture2D& texture) const
  {
    bind();
    mNormalMapId = texture.getId();
    glUniform1i(mNormalMapLocation, NORMAL_MAP_UNIT);
  }

  void WaterShaderProgram::setLightPos(const glm::vec3& pos) const
  {
    bind();
    glUniform3fv(mLightPos, 1, glm::value_ptr(pos));
  }

  void WaterShaderProgram::setLightColor(const glm::vec3& color) const
  {
    bind();
    glUniform3fv(mLightColor, 1, glm::value_ptr(color));
  }
}
