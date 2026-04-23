#include "WaterShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#ifdef __gl_h_
#undef __gl_h_
#endif
#include <glad/glad.h>

#include "Camera.h"
#include "Viewport.h"

namespace RenderSystem
{
  WaterShaderProgram::WaterShaderProgram(
    const std::filesystem::path& vertexShaderPath,
    const std::filesystem::path& fragmentShaderPath
  )
    : ShaderProgram(vertexShaderPath, fragmentShaderPath)
  {
    initUniformLocations();
  }

  void WaterShaderProgram::onCameraPosChanged(Camera* camera)
  {
    setCameraPos(camera->getEye());
    setView(camera->getViewMatrix());
  }

  void WaterShaderProgram::onViewportChanged(Viewport* viewport)
  {
    setProjection(viewport->getProjectionMatrix());
  }

  void WaterShaderProgram::initUniformLocations()
  {
    mModel = getUniformLocation("model");
    mView = getUniformLocation("view");
    mProjection = getUniformLocation("projection");
    mVTime = getUniformLocation("vTime");
    mFTime = getUniformLocation("fTime");
    mWaveCount = getUniformLocation("waveCount");
    mSkybox = getUniformLocation("skybox");
    mNormalMap = getUniformLocation("normalMap");
    mNormalMapMoves = getUniformLocation("normalMapMoves");
    mNormalMapMoveCount = getUniformLocation("normalMapMoveCount");
    mNormalStrength = getUniformLocation("normalStrength");
    mDepthFalloff = getUniformLocation("depthFalloff");
    mFresnelPower = getUniformLocation("fresnelPower");
    mReflectionIntensity = getUniformLocation("reflectionIntensity");
    mCameraPos = getUniformLocation("cameraPos");
    mDeepColor = getUniformLocation("deepColor");
    mShallowColor = getUniformLocation("shallowColor");
  }

  void WaterShaderProgram::setModel(const glm::mat4& model)
  {
    invoke([this, &model]()
           { glUniformMatrix4fv(mModel, 1, false, glm::value_ptr(model)); });
  }

  void WaterShaderProgram::setCameraPos(const glm::vec3& cameraPos)
  {
    invoke([this, &cameraPos]()
           { glUniform3fv(mCameraPos, 1, glm::value_ptr(cameraPos)); });
  }

  void WaterShaderProgram::setView(const glm::mat4& view)
  {
    invoke([this, &view]() { glUniformMatrix4fv(mView, 1, false, glm::value_ptr(view)); }
    );
  }

  void WaterShaderProgram::setProjection(const glm::mat4& projection)
  {
    invoke([this, &projection]()
           { glUniformMatrix4fv(mProjection, 1, false, glm::value_ptr(projection)); });
  }

  void WaterShaderProgram::setVTime(float time)
  {
    invoke([this, time]() { glUniform1f(mVTime, time); });
  }

  void WaterShaderProgram::setFTime(float time)
  {
    invoke([this, time]() { glUniform1f(mFTime, time); });
  }

  void WaterShaderProgram::setWaves(const std::vector<Wave>& waves)
  {
    invoke(
      [this, &waves]()
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
    );
  }

  void WaterShaderProgram::setNormalMapMoves(const std::vector<glm::vec2>& normalMapMoves)
  {
    invoke(
      [this, &normalMapMoves]()
      {
        glUniform1i(mNormalMapMoveCount, normalMapMoves.size());
        glUniform2fv(
          mNormalMapMoves, normalMapMoves.size(), glm::value_ptr(normalMapMoves[0])
        );
      }
    );
  }

  void WaterShaderProgram::setSkyboxCubemap(const CubemapTexture& texture) const
  {
    invoke([this, &texture]() { texture.passToFragmentShader(mSkybox, 0); });
  }

  void WaterShaderProgram::setNormalMap(const ImageTexture& texture)
  {
    invoke([this, &texture]() { texture.passToFragmentShader(mNormalMap, 1); });
  }

  void WaterShaderProgram::setNormalStrength(float normalStrength)
  {
    invoke([this, normalStrength]() { glUniform1f(mNormalStrength, normalStrength); });
  }

  void WaterShaderProgram::setDepthFalloff(float depthFalloff)
  {
    invoke([this, depthFalloff]() { glUniform1f(mDepthFalloff, depthFalloff); });
  }

  void WaterShaderProgram::setFresnelPower(float fresnelPower)
  {
    invoke([this, fresnelPower]() { glUniform1f(mFresnelPower, fresnelPower); });
  }

  void WaterShaderProgram::setReflectionIntensity(float reflectionIntensity)
  {
    invoke([this, reflectionIntensity]()
           { glUniform1f(mReflectionIntensity, reflectionIntensity); });
  }

  void WaterShaderProgram::setDeepColor(const glm::vec3& deepColor)
  {
    invoke([this, &deepColor]()
           { glUniform3fv(mDeepColor, 1, glm::value_ptr(deepColor)); });
  }

  void WaterShaderProgram::setShallowColor(const glm::vec3& shallowColor)
  {
    invoke([this, &shallowColor]()
           { glUniform3fv(mShallowColor, 1, glm::value_ptr(shallowColor)); });
  }
}
